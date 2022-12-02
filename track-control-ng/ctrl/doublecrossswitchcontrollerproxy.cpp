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
	statechart(nullptr),
	part(new_part)
{
	ControllerRegistry::instance().registerController(part, this);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::inquire,
		&statechart, &SwitchStatechart::inquire,
		Qt::QueuedConnection);
	connect(
		&statechart, &SwitchStatechart::entered, [&]()
	{
		ControllerRegistry::instance().increase(this);
		qDebug().noquote() << part->toString() << "Inquiry started.";
	});
	connect(
		&statechart, &SwitchStatechart::inquired, [&]()
	{
		ControllerRegistry::instance().decrease(this);
		qDebug().noquote() << part->toString() << "Inquiry completed.";
	});

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);
	statechart.enter();

}

DoubleCrossSwitchControllerProxy::~DoubleCrossSwitchControllerProxy()
{
	statechart.exit();
	ControllerRegistry::instance().unregisterController(part);
}

void DoubleCrossSwitchControllerProxy::turnLeft()
{
	part->setState(DoubleCrossSwitch::State::AC);
	statechart.turn();
}

void DoubleCrossSwitchControllerProxy::turn()
{
	part->setState(isTurnedLeft() ?
		DoubleCrossSwitch::State::BC :
		DoubleCrossSwitch::State::AC);
	statechart.turn();
}

void DoubleCrossSwitchControllerProxy::turnRight()
{
	part->setState(DoubleCrossSwitch::State::BC);
	statechart.turn();
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
	return part->section()->occupation() ? SectionState::OCCUPIED : SectionState::FREE;
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
			statechart.leftResponse();
			emit update();
			return true;

		case SETRGT:
			if (part->switchState() != SwitchState::SWITCH_STATE_RIGHT)
			{
				part->setState(DoubleCrossSwitch::State::BC);
			}
			statechart.rightResponse();
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

bool DoubleCrossSwitchControllerProxy::isTurnedLeft()
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
