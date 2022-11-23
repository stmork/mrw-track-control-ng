//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/regularswitchcontroller.h>

using namespace mrw::ctrl;

RegularSwitchController::RegularSwitchController(QObject * parent) :
	BaseController(parent)
{
}

float RegularSwitchController::extensions() const
{
	return 0;
}
