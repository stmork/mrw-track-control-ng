//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/regularswitchcontrollerproxy.h>
#include <model/region.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;

RegularSwitchControllerProxy::RegularSwitchControllerProxy(QObject * parent) :
	RegularSwitchController(parent)
{
}

void RegularSwitchControllerProxy::setSwitch(mrw::model::RegularSwitch * new_part)
{
	Q_ASSERT(part == nullptr);

	part = new_part;
}

bool RegularSwitchControllerProxy::isLeft() const
{
	return part->state() == RegularSwitch::State::AB;
}

bool RegularSwitchControllerProxy::isRight() const
{
	return part->state() == RegularSwitch::State::AC;
}

bool RegularSwitchControllerProxy::isRightHanded() const
{
	return part->isRightHanded();
}

bool RegularSwitchControllerProxy::isDirection() const
{
	return part->aIsDir() != part->section()->region()->direction();
}

QString RegularSwitchControllerProxy::name() const
{
	return part->name();
}