//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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

void DoubleCrossSwitchController::status(DoubleCrossSwitchController::Status & status) const
{
	BaseSwitchController::status(status);

	status.state    = switchState();

	const unsigned state = (unsigned)status.state;

	const unsigned b_masked = state & DoubleCrossSwitch::B_MASK;
	const unsigned d_masked = state & DoubleCrossSwitch::D_MASK;
	const unsigned b_mask   = status.right_bended ? DoubleCrossSwitch::B_MASK : 0;
	const unsigned d_mask   = status.right_bended ? DoubleCrossSwitch::D_MASK : 0;

	status.is_a = b_masked != b_mask;
	status.is_b = b_masked == b_mask;
	status.is_c = d_masked != d_mask;
	status.is_d = d_masked == d_mask;
}
