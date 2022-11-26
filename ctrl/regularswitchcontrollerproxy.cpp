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

RegularSwitchControllerProxy::RegularSwitchControllerProxy(
	RegularSwitch * new_part,
	QObject    *    parent) :
	RegularSwitchController(parent),
	part(new_part)
{
	ControllerRegistry::instance().registerController(part, this);
}

mrw::ctrl::RegularSwitchControllerProxy::~RegularSwitchControllerProxy()
{
	ControllerRegistry::instance().unregisterController(part);
}

QString RegularSwitchControllerProxy::name() const
{
	return part->name();
}

bool RegularSwitchControllerProxy::isDirection() const
{
	return part->aIsDir() != part->section()->region()->direction();
}

Position * RegularSwitchControllerProxy::position() const
{
	return part;
}

SectionState RegularSwitchControllerProxy::state() const
{
	return part->section()->state();
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
