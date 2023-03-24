//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/baseswitchcontroller.h>

using namespace mrw::ctrl;

BaseSwitchController::BaseSwitchController(QObject * parent) :
	BaseController(parent)
{
}

void BaseSwitchController::status(BaseSwitchController::Status & status)
{
	BaseController::status(status);

	status.right_bended         = isRightBended();
	status.has_flank_protection = hasFlankProtection();
}
