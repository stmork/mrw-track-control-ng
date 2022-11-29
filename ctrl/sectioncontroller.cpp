//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

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

bool SectionController::process(const mrw::can::MrwMessage & message)
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
	const MrwMessage command(GETRBS, ctrl_section->controller()->id(), ctrl_section->unitNo());

	ControllerRegistry::can()->write(command);
}

void SectionController::on()
{
	const MrwMessage command(SETRON, ctrl_section->controller()->id(), ctrl_section->unitNo());

	ControllerRegistry::can()->write(command);
}

void SectionController::off()
{
	const MrwMessage command(SETROF, ctrl_section->controller()->id(), ctrl_section->unitNo());

	ControllerRegistry::can()->write(command);
}

SectionController::operator Section * () const
{
	return ctrl_section;
}
