//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontroller.h>

using namespace mrw::ctrl;

DoubleCrossSwitchController::DoubleCrossSwitchController(QObject * parent) :
	BaseController(parent)
{
}

float mrw::ctrl::DoubleCrossSwitchController::extensions() const
{
	return 0.0;
}
