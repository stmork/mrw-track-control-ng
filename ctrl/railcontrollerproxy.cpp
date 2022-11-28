//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPoint>

#include <can/commands.h>
#include <ctrl/railcontrollerproxy.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;

RailControllerProxy::RailControllerProxy(
	Section * parent_section,
	Rail   *  referenced_rail,
	QObject * parent) :
	RailController(parent),
	section(parent_section),
	rail(referenced_rail)
{
}

QString RailControllerProxy::name() const
{
	return rail->partName();
}

float RailControllerProxy::extensions() const
{
	return rail->extension();
}

float mrw::ctrl::RailControllerProxy::lines() const
{
	return rail->lines();
}

bool RailControllerProxy::isDirection() const
{
	return !rail->aIsDir() == section->region()->direction();
}

bool RailControllerProxy::isExpandable() const
{
	return true;
}

Position * RailControllerProxy::position() const
{
	return rail;
}

SectionState RailControllerProxy::state() const
{
	return section->state();
}

Device::LockState RailControllerProxy::lock() const
{
	return section->lock();
}

Position::Bending RailControllerProxy::bending() const
{
	return rail->bending();
}

bool RailControllerProxy::aEnds() const
{
	return rail->advance(rail->aIsDir()).empty();
}

bool RailControllerProxy::bEnds() const
{
	return rail->advance(!rail->aIsDir()).empty();
}
