//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "regularswitchcontrollerproxy.h"

using namespace mrw::can;
using namespace mrw::ctrl;

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
	return part->state() == SwitchState::SWITCH_STATE_LEFT;
}

bool RegularSwitchControllerProxy::isRight() const
{
	return part->state() == SwitchState::SWITCH_STATE_RIGHT;
}

bool RegularSwitchControllerProxy::isRightHanded() const
{
	return part->isRightHanded();
}

bool RegularSwitchControllerProxy::isDirection() const
{
	return !part->aIsDir();
}

QString RegularSwitchControllerProxy::name() const
{
	return part->name();
}
