//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPoint>

#include <can/commands.h>
#include <ctrl/sectioncontrollerproxy.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;

SectionControllerProxy::SectionControllerProxy(
	Section * parent_section,
	Rail   *  referenced_rail,
	QObject * parent) :
	SectionController(parent),
	section(parent_section),
	rail(referenced_rail)
{
}

QString SectionControllerProxy::name() const
{
	return section->name();
}

float SectionControllerProxy::extensions() const
{
	return rail->extension();
}

bool SectionControllerProxy::isDirection() const
{
	return rail->aIsDir() == section->region()->direction();
}

bool SectionControllerProxy::isExpandable() const
{
	return true;
}

Position * SectionControllerProxy::position() const
{
	return rail;
}

SectionState SectionControllerProxy::state() const
{
	return section->state();
}

Device::LockState SectionControllerProxy::lock() const
{
	return section->lock();
}

Position::Bending SectionControllerProxy::bending() const
{
	return rail->bending();
}

bool SectionControllerProxy::aEnds() const
{
	return rail->advance(true).empty();
}

bool SectionControllerProxy::bEnds() const
{
	return rail->advance(false).empty();
}
