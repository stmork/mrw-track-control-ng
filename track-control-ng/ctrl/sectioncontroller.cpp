//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include <util/method.h>
#include <can/mrwmessage.h>
#include <model/railpart.h>
#include <ctrl/sectioncontroller.h>
#include <ctrl/controllerregistry.h>
#include <statecharts/timerservice.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::statechart;

using LockState = Section::LockState;

SectionController::SectionController(
	Section * input,
	QObject * parent) :
	BaseController(parent),
	statechart(nullptr),
	ctrl_section(input)
{
	ControllerRegistry::instance().registerController(ctrl_section, this);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::clear,
		&statechart, &SectionStatechart::clear,
		Qt::DirectConnection);
	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::start,
		&statechart, &SectionStatechart::start,
		Qt::QueuedConnection);
	connect(
		this, &SectionController::enable,
		&statechart, &SectionStatechart::enable,
		Qt::QueuedConnection);
	connect(
		this, &SectionController::disable,
		&statechart, &SectionStatechart::disable,
		Qt::QueuedConnection);
	connect(
		this, &SectionController::failed,
		&statechart, &SectionStatechart::failed,
		Qt::QueuedConnection);
	connect(
		this, &SectionController::start,
		&statechart, &SectionStatechart::start,
		Qt::QueuedConnection);
	connect(
		&statechart, &SectionStatechart::left,
		this, &SectionController::left,
		Qt::DirectConnection);
	connect(
		&statechart, &SectionStatechart::tryUnblock,
		this, &SectionController::tryUnblock,
		Qt::DirectConnection);
	connect(
		&statechart, &SectionStatechart::unregister,
		this, &SectionController::unregister,
		Qt::DirectConnection);
	connect(
		&statechart, &SectionStatechart::entered, [&]()
	{
		qDebug().noquote() << ctrl_section->toString() << "Inquiry started.";
	});
	connect(
		&statechart, &SectionStatechart::started, [&]()
	{
		qDebug().noquote() << ctrl_section->toString() << "Inquiry completed.";
	});

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);
	statechart.enter();
}

SectionController::~SectionController()
{
	statechart.exit();
	ControllerRegistry::instance().unregisterController(ctrl_section);
}

Section * SectionController::section() const
{
	return *this;
}

void SectionController::setAutoUnlock(const bool auto_unlock)
{
	statechart.setAuto_unlock(auto_unlock);
}

void SectionController::inc()
{
	ControllerRegistry::instance().increase(this);
}

void SectionController::dec()
{
	ControllerRegistry::instance().decrease(this);
}

QString SectionController::name() const
{
	return ctrl_section->name();
}

float SectionController::extensions() const
{
	return position()->extension();
}

bool SectionController::isDirection() const
{
	// A section does not have any direction.
	return false;
}

bool SectionController::isExpandable() const
{
	// A section itself is not editable.
	return false;
}

Position * SectionController::position() const
{
	return ctrl_section;
}

SectionState SectionController::state() const
{
	return ctrl_section->occupation() ? SectionState::OCCUPIED : SectionState::FREE;
}

Device::LockState SectionController::lock() const
{
	return ctrl_section->lock();
}

Position::Bending SectionController::bending() const
{
	return position()->bending();
}

bool SectionController::process(const MrwMessage & message)
{
	qDebug().noquote() << message << "(Section)";

	if (message.response() != Response::MSG_OK)
	{
		statechart.failed();
		return true;
	}

	switch (message.command())
	{
	case SETRON:
		statechart.relaisResponse();
		return true;

	case SETROF:
		statechart.relaisResponse();
		return true;

	case GETRBS:
		ctrl_section->setOccupation(message[0] != 0);
		statechart.stateResponse(ctrl_section->occupation());
		emit update();
		return true;

	default:
		break;
	}
	return false;
}

void SectionController::restart()
{
	statechart.clear();
	emit start();
}

QString SectionController::toString() const
{
	return *section();
}

void SectionController::request()
{
	const MrwMessage command(ctrl_section->command(GETRBS));

	ControllerRegistry::can()->write(command);
}

void SectionController::passed()
{
	section()->setState(SectionState::PASSED);
	emit update();
}

void SectionController::free()
{
	section()->free();
	emit update();
}

void SectionController::fail()
{
	qCritical().noquote() << String::red(" Section failed!") << name();

	section()->setLock(LockState::FAIL);
	ControllerRegistry::instance().failed();
	emit update();
}

void SectionController::pending()
{
	section()->setLock(LockState::PENDING);
	emit update();
}

void SectionController::lock(const bool do_it)
{
	section()->setLock(do_it ? LockState::LOCKED : LockState::UNLOCKED);
	emit update();
}

void SectionController::on()
{
	const MrwMessage command(ctrl_section->command(SETRON));

	ControllerRegistry::can()->write(command);
}

void SectionController::off()
{
	const MrwMessage command(ctrl_section->command(SETROF));

	ControllerRegistry::can()->write(command);
}

SectionController::operator Section * () const
{
	return ctrl_section;
}
