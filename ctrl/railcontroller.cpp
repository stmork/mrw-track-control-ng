//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/railcontroller.h>

using namespace mrw::ctrl;

RailController::RailController(QObject * parent) :
	BaseController(parent)
{
}

void RailController::status(RailController::Status & status)
{
	BaseController::status(status);

	status.a_ends = aEnds();
	status.b_ends = bEnds();
}
