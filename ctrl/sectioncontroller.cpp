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
		&statechart, &SectionStatechart::inquire);
	connect(
		&statechart, &SectionStatechart::entered, [&]()
	{
		ControllerRegistry::instance().increase(this);
		qDebug().noquote() << ctrl_section->toString() << "Inquiry started.";
	});
	connect(
		&statechart, &SectionStatechart::inquired, [&]()
	{
		ControllerRegistry::instance().decrease(this);
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
		statechart.relaisResponse(true);
		return true;

	case SETROF:
		statechart.relaisResponse(false);
		return true;

	case GETRBS:
		ctrl_section->setOccupation(message[0] != 0);
		statechart.stateResponse();
		emit update();
		return true;

	default:
		break;
	}
	return false;
}

void SectionController::request()
{
	__METHOD__;

	const MrwMessage command(GETRBS, ctrl_section->controller()->id(), ctrl_section->unitNo());

	ControllerRegistry::can()->write(command);
}

void SectionController::on()
{
	__METHOD__;

	const MrwMessage command(SETRON, ctrl_section->controller()->id(), ctrl_section->unitNo());

	ControllerRegistry::can()->write(command);
}

void SectionController::off()
{
	__METHOD__;

	const MrwMessage command(SETROF, ctrl_section->controller()->id(), ctrl_section->unitNo());

	ControllerRegistry::can()->write(command);
}

SectionController::operator Section * () const
{
	return ctrl_section;
}
