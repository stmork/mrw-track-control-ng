//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>
#include <QThread>

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
		QCoreApplication::processEvents();

		qCDebug(log).noquote() << part->toString() << "Inquiry started.";
	});
	connect(
		&statechart, &SwitchStatechart::started, [&]()
	{
		qCDebug(log).noquote() << part->toString() << "Inquiry completed.";
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

const QString & DoubleCrossSwitchControllerProxy::name() const noexcept
{
	return part->name();
}

bool DoubleCrossSwitchControllerProxy::isDirection() const noexcept
{
	return !part->aIsDir() == part->section()->region()->direction();
}

Position * DoubleCrossSwitchControllerProxy::position() const noexcept
{
	return part;
}

SectionState DoubleCrossSwitchControllerProxy::state() const noexcept
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

Device::LockState DoubleCrossSwitchControllerProxy::lock() const noexcept
{
	return part->lock();
}

Position::Bending DoubleCrossSwitchControllerProxy::bending() const noexcept
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
	qCDebug(log).noquote() << message << "(double cross switch)";

	switch (message.response())
	{
	case Response::MSG_QUEUED:
		statechart.queued();
		return true;

	case Response::MSG_OK:
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
			if (part->setState(message[0] != 3 ? DoubleCrossSwitch::State(message[0] - 1u) : DoubleCrossSwitch::State::FAIL, true))
			{
				statechart.response();
			}
			else
			{
				statechart.failed();
			}
			emit update();
			return true;

		default:
			// Intentionally do nothing.
			break;
		}
		break;

	default:
		qCCritical(log).noquote() << "Error switching" << part->toString();
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
	qCCritical(log).noquote() << String::red(" Switch turn failed!") << name();

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
