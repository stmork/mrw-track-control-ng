//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <ctrl/railcontroller.h>

using namespace mrw::ctrl;

RailController::RailController(QObject * parent) :
	BaseController(parent)
{
}

void RailController::status(RailController::Status & status) const
{
	BaseController::status(status);

	status.a_ends       = aEnds();
	status.b_ends       = bEnds();
	status.has_crossing = hasCrossing();
}

bool RailController::isExpandable() const
{
	return true;
}
