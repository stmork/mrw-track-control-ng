//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "regularswitchcontrollerproxy.h"

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
