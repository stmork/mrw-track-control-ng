//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontroller.h>

using namespace mrw::model;
using namespace mrw::ctrl;

DoubleCrossSwitchController::DoubleCrossSwitchController(QObject * parent) :
	BaseSwitchController(parent)
{
}

float DoubleCrossSwitchController::extensions() const
{
	return 0.0;
}

void DoubleCrossSwitchController::status(DoubleCrossSwitchController::Status & status)
{
	BaseSwitchController::status(status);

	status.state    = switchState();
	status.b_masked = (unsigned)status.state & DoubleCrossSwitch::B_MASK;
	status.d_masked = (unsigned)status.state & DoubleCrossSwitch::D_MASK;
}
