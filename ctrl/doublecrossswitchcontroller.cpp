//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontroller.h>

using namespace mrw::ctrl;

DoubleCrossSwitchController::DoubleCrossSwitchController(QObject * parent) :
	BaseSwitchController(parent)
{
}

float DoubleCrossSwitchController::extensions() const
{
	return 0.0;
}
