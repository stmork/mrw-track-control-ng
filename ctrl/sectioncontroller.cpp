//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/sectioncontroller.h>
#include <ctrl/controllerregistry.h>

using namespace mrw::model;
using namespace mrw::ctrl;

SectionController::SectionController(
	Section * input,
	QObject * parent) :
	BaseController(parent),
	ctrl_section(input)
{
	ControllerRegistry::instance().registerController(ctrl_section, this);
}

SectionController::~SectionController()
{
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
	return ctrl_section->state();
}

Device::LockState SectionController::lock() const
{
	return ctrl_section->lock();
}

Position::Bending SectionController::bending() const
{
	return position()->bending();
}

SectionController::operator Section * () const
{
	return ctrl_section;
}
