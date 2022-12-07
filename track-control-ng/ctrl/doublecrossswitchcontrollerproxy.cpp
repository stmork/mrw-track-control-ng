//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include <util/method.h>
#include <model/region.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/controllerregistry.h>
#include <statecharts/timerservice.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::statechart;

using LockState = Device::LockState;

DoubleCrossSwitchControllerProxy::DoubleCrossSwitchControllerProxy(
	DoubleCrossSwitch * new_part,
	QObject      *      parent) :
	DoubleCrossSwitchController(parent),
	part(new_part)
{
	ControllerRegistry::instance().registerController(part, this);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::inquire,
		&statechart, &SwitchStatechart::inquire,
		Qt::QueuedConnection);
	connect(
		this, &DoubleCrossSwitchControllerProxy::turn,
		&statechart, &SwitchStatechart::turn,
		Qt::QueuedConnection);
	connect(
		this, &DoubleCrossSwitchControllerProxy::leftResponse,
		&statechart, &SwitchStatechart::leftResponse,
		Qt::QueuedConnection);
	connect(
		this, &DoubleCrossSwitchControllerProxy::rightResponse,
		&statechart, &SwitchStatechart::rightResponse,
		Qt::QueuedConnection);
	connect(
		this, &DoubleCrossSwitchControllerProxy::unlock,
		&statechart, &SwitchStatechart::unlock,
		Qt::QueuedConnection);
	connect(
		&statechart, &SwitchStatechart::entered, [&]()
	{
		qDebug().noquote() << part->toString() << "Inquiry started.";
	});
	connect(
		&statechart, &SwitchStatechart::inquired, [&]()
	{
		qDebug().noquote() << part->toString() << "Inquiry completed.";
	});
}

DoubleCrossSwitchControllerProxy::~DoubleCrossSwitchControllerProxy()
{
	ControllerRegistry::instance().unregisterController(part);
}

void DoubleCrossSwitchControllerProxy::turnLeft()
{
	part->setState(DoubleCrossSwitch::State::AC);
	emit turn();
}

void mrw::ctrl::DoubleCrossSwitchControllerProxy::change()
{
	part->setState(doTurnLeft() ?
		DoubleCrossSwitch::State::BC :
		DoubleCrossSwitch::State::AC);
	emit turn();
}

void DoubleCrossSwitchControllerProxy::turnRight()
{
	part->setState(DoubleCrossSwitch::State::BC);
	emit turn();
}

QString DoubleCrossSwitchControllerProxy::name() const
{
	return part->name();
}

bool DoubleCrossSwitchControllerProxy::isDirection() const
{
	return !part->aIsDir() == part->section()->region()->direction();
}

Position * DoubleCrossSwitchControllerProxy::position() const
{
	return part;
}

SectionState DoubleCrossSwitchControllerProxy::state() const
{
	if (part->reserved())
	{
		return part->section()->occupation() ? SectionState::OCCUPIED : section()->state();
	}
	else
	{
		return SectionState::FREE;
	}
}

Device::LockState DoubleCrossSwitchControllerProxy::lock() const
{
	return part->lock();
}

Position::Bending DoubleCrossSwitchControllerProxy::bending() const
{
	return part->bending();
}

DoubleCrossSwitch::State DoubleCrossSwitchControllerProxy::switchState() const
{
	return part->state();
}

RailPart * DoubleCrossSwitchControllerProxy::railPart() const
{
	return part;
}

bool DoubleCrossSwitchControllerProxy::process(const MrwMessage & message)
{
	qDebug().noquote() << message << "(double cross switch)";

	switch (message.response())
	{
	case MSG_QUEUED:
		statechart.queued();
		return true;

	case MSG_OK:
		switch (message.command())
		{
		case SETLFT:
			if (part->switchState() != SwitchState::SWITCH_STATE_LEFT)
			{
				part->setState(DoubleCrossSwitch::State::AC);
			}
			emit leftResponse();
			emit update();
			return true;

		case SETRGT:
			if (part->switchState() != SwitchState::SWITCH_STATE_RIGHT)
			{
				part->setState(DoubleCrossSwitch::State::BC);
			}
			emit rightResponse();
			emit update();
			return true;

		case GETDIR:
			part->setState(DoubleCrossSwitch::State(message[0]));
			statechart.response();
			emit update();
			return true;

		default:
			// Intentionally do nothing.
			break;
		}
		break;

	default:
		qCritical().noquote() << "Error switching" << part->toString();
		statechart.failed();
		break;
	}
	return false;
}

QString mrw::ctrl::DoubleCrossSwitchControllerProxy::toString() const
{
	return *part;
}

void DoubleCrossSwitchControllerProxy::left()
{
	__METHOD__;

	const MrwMessage  command(part->command(SETLFT));

	part->setState(DoubleCrossSwitch::State::AC);
	ControllerRegistry::can()->write(command);
	emit update();
}

void DoubleCrossSwitchControllerProxy::right()
{
	__METHOD__;

	const MrwMessage  command(part->command(SETRGT));

	part->setState(DoubleCrossSwitch::State::BC);
	ControllerRegistry::can()->write(command);
	emit update();
}

void DoubleCrossSwitchControllerProxy::request()
{
	__METHOD__;

	const MrwMessage  command(part->command(GETDIR));

	ControllerRegistry::can()->write(command);
}

bool DoubleCrossSwitchControllerProxy::doTurnLeft()
{
	__METHOD__;

	return
		(switchState() == DoubleCrossSwitch::State::AC) ||
		(switchState() == DoubleCrossSwitch::State::BD);
}

bool DoubleCrossSwitchControllerProxy::isFree()
{
	__METHOD__;

	return state() == SectionState::FREE;
}

void DoubleCrossSwitchControllerProxy::fail()
{
	__METHOD__;

	part->setLock(LockState::FAIL);
	emit update();
}

void DoubleCrossSwitchControllerProxy::pending()
{
	__METHOD__;

	part->setLock(LockState::PENDING);
	emit update();
}

void DoubleCrossSwitchControllerProxy::lock(bool do_it)
{
	__METHOD__;

	part->setLock(do_it ? LockState::LOCKED : LockState::UNLOCKED);
	emit update();
}
