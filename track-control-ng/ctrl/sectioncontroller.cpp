//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include <util/method.h>
#include <can/mrwmessage.h>
#include <ctrl/sectioncontroller.h>
#include <ctrl/controllerregistry.h>
#include <statecharts/timerservice.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::statechart;

SectionController::SectionController(
	Section * input,
	QObject * parent) :
	BaseController(parent),
	statechart(nullptr),
	ctrl_section(input)
{
	ControllerRegistry::instance().registerController(ctrl_section, this);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::inquire,
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
		&statechart, &SectionStatechart::left,
		this, &SectionController::left,
		Qt::QueuedConnection);
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

QString SectionController::toString() const
{
	return *section();
}

void SectionController::request()
{
	__METHOD__;

	const MrwMessage command(ctrl_section->command(GETRBS));

	ControllerRegistry::can()->write(command);
}

void SectionController::passed()
{
	__METHOD__;

	section()->setState(SectionState::PASSED);
}

void SectionController::lock(const bool do_it)
{
	__METHOD__;

	section()->setLock(do_it ? Section::LockState::LOCKED : Section::LockState::UNLOCKED);
}

void SectionController::on()
{
	__METHOD__;

	const MrwMessage command(ctrl_section->command(SETRON));

	ControllerRegistry::can()->write(command);
}

void SectionController::off()
{
	__METHOD__;

	const MrwMessage command(ctrl_section->command(SETROF));

	ControllerRegistry::can()->write(command);
}

SectionController::operator Section * () const
{
	return ctrl_section;
}
