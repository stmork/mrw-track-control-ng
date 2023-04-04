//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QDebug>

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
		&ControllerRegistry::instance(), &ControllerRegistry::clear,
		&statechart, &SwitchStatechart::clear,
		Qt::DirectConnection);
	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::start,
		&statechart, &SwitchStatechart::start,
		Qt::QueuedConnection);
	connect(
		&statechart, &SwitchStatechart::stop,
		this, &DoubleCrossSwitchControllerProxy::stop,
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
		this, &DoubleCrossSwitchControllerProxy::failed,
		&statechart, &SwitchStatechart::failed,
		Qt::QueuedConnection);
	connect(
		this, &DoubleCrossSwitchControllerProxy::start,
		&statechart, &SwitchStatechart::start,
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
		&statechart, &SwitchStatechart::started, [&]()
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

void DoubleCrossSwitchControllerProxy::change()
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

const QString & DoubleCrossSwitchControllerProxy::name() const
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
		if (section()->anyReserved())
		{
			return SectionState::FREE;
		}
		else
		{
			return part->section()->occupation() ? SectionState::OCCUPIED : section()->state();
		}
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

bool DoubleCrossSwitchControllerProxy::isRightBended() const
{
	return part->bending() == Position::Bending::RIGHT;
}

bool DoubleCrossSwitchControllerProxy::hasFlankProtection() const
{
	if ((lock() == LockState::LOCKED) && isFlankRelevantReserved())
	{
		std::vector<RegularSwitch *> flank_switches;

		const size_t equal = part->flank(flank_switches);

		return (flank_switches.size() > 0) && (flank_switches.size() == equal);
	}

	return false;
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
			if (part->commandState() != message.command())
			{
				part->setState(DoubleCrossSwitch::State::AC);
			}
			emit leftResponse();
			emit update();
			return true;

		case SETRGT:
			if (part->commandState() != message.command())
			{
				part->setState(DoubleCrossSwitch::State::BC);
			}
			emit rightResponse();
			emit update();
			return true;

		case GETDIR:
			part->setState(DoubleCrossSwitch::State(message[0] - 1u), true);
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

void DoubleCrossSwitchControllerProxy::restart()
{
	statechart.clear();
	emit start();
}

QString DoubleCrossSwitchControllerProxy::toString() const
{
	return *part;
}

void DoubleCrossSwitchControllerProxy::left()
{
	const MrwMessage  command(part->command(SETLFT));

	ControllerRegistry::can()->write(command);
	emit update();
}

void DoubleCrossSwitchControllerProxy::right()
{
	const MrwMessage  command(part->command(SETRGT));

	ControllerRegistry::can()->write(command);
	emit update();
}

void DoubleCrossSwitchControllerProxy::request()
{
	const MrwMessage  command(part->command(GETDIR));

	ControllerRegistry::can()->write(command);
}

bool DoubleCrossSwitchControllerProxy::doTurnLeft()
{
	return
		(switchState() == DoubleCrossSwitch::State::AC) ||
		(switchState() == DoubleCrossSwitch::State::BD);
}

bool DoubleCrossSwitchControllerProxy::isFree()
{
	return section()->isFree();
}

void DoubleCrossSwitchControllerProxy::fail()
{
	qCritical().noquote() << String::red(" Switch turn failed!") << name();

	part->setLock(LockState::FAIL);
	ControllerRegistry::instance().failed();
	emit update();
}

void DoubleCrossSwitchControllerProxy::pending()
{
	part->setLock(LockState::PENDING);
	emit update();
}

void DoubleCrossSwitchControllerProxy::lock(bool do_it)
{
	part->setLock(do_it ? LockState::LOCKED : LockState::UNLOCKED);
	emit update();
}
