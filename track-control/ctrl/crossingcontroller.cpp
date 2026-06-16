//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <ctrl/crossingcontroller.h>
#include <ctrl/controllerregistry.h>
#include <statecharts/timerservice.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::statechart;

using LockState = Crossing::LockState;

CrossingController::CrossingController(
	mrw::model::Crossing * input,
	QObject        *       parent) :
	BaseController(parent),
	crossing(input)
{
	ControllerRegistry::instance().registerController(crossing, this);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::clear,
		&statechart, &CrossingStatechart::clear,
		Qt::DirectConnection);
	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::start,
		&statechart, &CrossingStatechart::start,
		Qt::QueuedConnection);

	connect(
		&statechart, &CrossingStatechart::failed,
		this, &CrossingController::failed,
		Qt::QueuedConnection);

	connect(
		this, &CrossingController::action,
		&statechart, &CrossingStatechart::action,
		Qt::QueuedConnection);

	statechart.setTimerService(TimerService::instance());
	statechart.setOperationCallback(*this);

	Q_ASSERT(statechart.check());
	statechart.enter();
}

CrossingController::~CrossingController()
{
	statechart.exit();
	ControllerRegistry::instance().unregisterController(crossing);
}

const QString & CrossingController::name() const noexcept
{
	return crossing->name();
}

float CrossingController::extensions() const noexcept
{
	return 0.0;
}

bool CrossingController::isDirection() const noexcept
{
	return false;
}

bool mrw::ctrl::CrossingController::isExpandable() const noexcept
{
	return false;
}

Position * CrossingController::position() const noexcept
{
	return nullptr;
}

SectionState mrw::ctrl::CrossingController::state() const noexcept
{
	return SectionState::FREE;
}

Device::LockState CrossingController::lock() const noexcept
{
	return crossing->lock();
}

Position::Bending CrossingController::bending() const noexcept
{
	return Position::Bending::STRAIGHT;
}

bool CrossingController::process(const MrwMessage & message) noexcept
{
	qCDebug(log).noquote() << message << "(Crossing)" << name();

	switch (message.response())
	{
	case Response::MSG_QUEUED:
		return true;

	case Response::MSG_OK:
		if (message.command() == SETSGN)
		{
			statechart.response();
			return true;
		}
		break;

	default:
		break;
	}
	return false;
}

void CrossingController::restart()
{
	statechart.clear();
	emit start();
}

QString CrossingController::toString() const
{
	return crossing->name();
}

void CrossingController::inc()
{
	increase();
}

void CrossingController::dec()
{
	decrease();
}

void mrw::ctrl::CrossingController::unregister()
{
	setBatch(nullptr);
}

bool CrossingController::used()
{
	qCDebug(log).noquote() << "(Crossing)" << name() << "used:" << crossing->isUsed();
	return crossing->isUsed();
}

void CrossingController::close()
{
	MrwMessage message(crossing->command(SETSGN));

	message.append(std::underlying_type_t<SignalAspect>(SignalAspect::SIGNAL_CRX));
	ControllerRegistry::can()->write(message);
}

void CrossingController::open()
{
	MrwMessage message(crossing->command(SETSGN));

	message.append(std::underlying_type_t<SignalAspect>(SignalAspect::SIGNAL_OFF));
	ControllerRegistry::can()->write(message);
}

void CrossingController::fail()
{
	qCCritical(log).noquote() << String::red(" Crossing failed!") << name();

	crossing->setLock(LockState::FAIL);
	ControllerRegistry::instance().failed();
	emit update();
}

void CrossingController::pending()
{
	crossing->setLock(LockState::PENDING);
	emit update();
}

void CrossingController::lock(const bool do_it)
{
	crossing->setLock(do_it ? LockState::LOCKED : LockState::UNLOCKED);
	emit update();
}
