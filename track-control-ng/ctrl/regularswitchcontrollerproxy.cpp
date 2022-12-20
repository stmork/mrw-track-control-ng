//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/region.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/controllerregistry.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;
using namespace mrw::statechart;

using LockState = Device::LockState;

RegularSwitchControllerProxy::RegularSwitchControllerProxy(
	RegularSwitch * new_part,
	QObject    *    parent) :
	RegularSwitchController(parent),
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
		this, &RegularSwitchControllerProxy::stop,
		Qt::QueuedConnection);
	connect(
		this, &RegularSwitchControllerProxy::turn,
		&statechart, &SwitchStatechart::turn,
		Qt::QueuedConnection);
	connect(
		this, &RegularSwitchControllerProxy::leftResponse,
		&statechart, &SwitchStatechart::leftResponse,
		Qt::QueuedConnection);
	connect(
		this, &RegularSwitchControllerProxy::rightResponse,
		&statechart, &SwitchStatechart::rightResponse,
		Qt::QueuedConnection);
	connect(
		this, &RegularSwitchControllerProxy::failed,
		&statechart, &SwitchStatechart::failed,
		Qt::QueuedConnection);
	connect(
		this, &RegularSwitchControllerProxy::start,
		&statechart, &SwitchStatechart::start,
		Qt::QueuedConnection);
	connect(
		this, &RegularSwitchControllerProxy::unlock,
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

RegularSwitchControllerProxy::~RegularSwitchControllerProxy()
{
	ControllerRegistry::instance().unregisterController(part);
}

void RegularSwitchControllerProxy::turnLeft()
{
	part->setState(RegularSwitch::State::AB);
	emit turn();
}

void RegularSwitchControllerProxy::change()
{
	part->setState(isLeft() ?
		RegularSwitch::State::AC :
		RegularSwitch::State::AB);
	emit turn();
}

void RegularSwitchControllerProxy::turnRight()
{
	part->setState(RegularSwitch::State::AC);
	emit turn();
}

QString RegularSwitchControllerProxy::name() const
{
	return part->name();
}

bool RegularSwitchControllerProxy::isDirection() const
{
	return !part->aIsDir() == part->section()->region()->direction();
}

bool RegularSwitchControllerProxy::isExpandable() const
{
	return true;
}

float RegularSwitchControllerProxy::extensions() const
{
	return part->extension();
}

Position * RegularSwitchControllerProxy::position() const
{
	return part;
}

SectionState RegularSwitchControllerProxy::state() const
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

Device::LockState RegularSwitchControllerProxy::lock() const
{
	return part->lock();
}

Position::Bending RegularSwitchControllerProxy::bending() const
{
	return part->bending();
}

bool RegularSwitchControllerProxy::isLeft() const
{
	return part->state() == RegularSwitch::State::AB;
}

bool RegularSwitchControllerProxy::isRight() const
{
	return part->state() == RegularSwitch::State::AC;
}

bool RegularSwitchControllerProxy::isRightBended() const
{
	return part->bending() == Position::Bending::RIGHT;
}

bool RegularSwitchControllerProxy::isInclined() const
{
	return part->isInclined();
}

RailPart * RegularSwitchControllerProxy::railPart() const
{
	return part;
}

bool RegularSwitchControllerProxy::process(const MrwMessage & message)
{
	qDebug().noquote() << message << "(regular switch)";

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
				part->setState(RegularSwitch::State::AB);
			}
			emit leftResponse();
			emit update();
			return true;

		case SETRGT:
			if (part->switchState() != SwitchState::SWITCH_STATE_RIGHT)
			{
				part->setState(RegularSwitch::State::AC);
			}
			emit rightResponse();
			emit update();
			return true;

		case GETDIR:
			part->setState(RegularSwitch::State(message[0]));
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

void mrw::ctrl::RegularSwitchControllerProxy::restart()
{
	statechart.clear();
	emit start();
}

QString RegularSwitchControllerProxy::toString() const
{
	return *part;
}

void RegularSwitchControllerProxy::left()
{
	const MrwMessage  command(part->command(SETLFT));

	ControllerRegistry::can()->write(command);
	emit update();
}

void RegularSwitchControllerProxy::right()
{
	const MrwMessage  command(part->command(SETRGT));

	ControllerRegistry::can()->write(command);
	emit update();
}

void RegularSwitchControllerProxy::request()
{
	const MrwMessage  command(part->command(GETDIR));

	ControllerRegistry::can()->write(command);
}

bool RegularSwitchControllerProxy::doTurnLeft()
{
	return isLeft();
}

bool RegularSwitchControllerProxy::isFree()
{
	return section()->isFree();
}

void RegularSwitchControllerProxy::fail()
{
	qCritical().noquote() << String::red(" Switch turn failed!") << name();

	part->setLock(LockState::FAIL);
	ControllerRegistry::instance().failed();
	emit update();
}

void RegularSwitchControllerProxy::pending()
{
	part->setLock(LockState::PENDING);
	emit update();
}

void RegularSwitchControllerProxy::lock(bool do_it)
{
	part->setLock(do_it ? LockState::LOCKED : LockState::UNLOCKED);
	emit update();
}
