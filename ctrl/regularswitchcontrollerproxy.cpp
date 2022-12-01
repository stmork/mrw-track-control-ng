//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include <util/method.h>
#include <model/region.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/controllerregistry.h>
#include <statecharts/timerservice.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;
using namespace mrw::statechart;

using LockState = Device::LockState;

RegularSwitchControllerProxy::RegularSwitchControllerProxy(
	RegularSwitch * new_part,
	QObject    *    parent) :
	RegularSwitchController(parent),
	statechart(nullptr),
	part(new_part)
{
	ControllerRegistry::instance().registerController(part, this);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::inquire,
		&statechart, &SwitchStatechart::inquire);
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

mrw::ctrl::RegularSwitchControllerProxy::~RegularSwitchControllerProxy()
{
	statechart.exit();
	ControllerRegistry::instance().unregisterController(part);
}

void mrw::ctrl::RegularSwitchControllerProxy::turnLeft()
{
	statechart.turnLeft();
}

void mrw::ctrl::RegularSwitchControllerProxy::turn()
{
	statechart.turn();
}

void mrw::ctrl::RegularSwitchControllerProxy::turnRight()
{
	statechart.turnRight();
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
	return part->section()->occupation() ? SectionState::OCCUPIED : SectionState::FREE;
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
			part->setState(RegularSwitch::State::AB);
			statechart.leftResponse();
			emit update();
			return true;

		case SETRGT:
			part->setState(RegularSwitch::State::AC);
			statechart.rightResponse();
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
		statechart.failed();
		break;
	}
	return false;
}

void RegularSwitchControllerProxy::left()
{
	__METHOD__;

	const MrwMessage  command(SETLFT, part->controller()->id(), part->unitNo());

	ControllerRegistry::can()->write(command);
}

void RegularSwitchControllerProxy::right()
{
	__METHOD__;

	const MrwMessage  command(SETRGT, part->controller()->id(), part->unitNo());

	ControllerRegistry::can()->write(command);
}

void RegularSwitchControllerProxy::request()
{
	__METHOD__;

	const MrwMessage  command(GETDIR, part->controller()->id(), part->unitNo());

	ControllerRegistry::can()->write(command);
}

bool RegularSwitchControllerProxy::isTurnedLeft()
{
	__METHOD__;

	return isLeft();
}

void mrw::ctrl::RegularSwitchControllerProxy::lock(bool do_it)
{
	__METHOD__;

	ControllerRegistry::instance().decrease(this);
	part->setLock(do_it ? LockState::LOCKED : LockState::UNLOCKED);
	emit update();
}

void mrw::ctrl::RegularSwitchControllerProxy::fail()
{
	__METHOD__;

	part->setLock(LockState::FAIL);
	emit update();
}

void mrw::ctrl::RegularSwitchControllerProxy::pending()
{
	__METHOD__;

	ControllerRegistry::instance().increase(this);
	part->setLock(LockState::PENDING);
	emit update();
}

bool RegularSwitchControllerProxy::isFree()
{
	__METHOD__;

	return state() == SectionState::FREE;
}
