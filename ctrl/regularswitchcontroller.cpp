//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/regularswitchcontroller.h>

using namespace mrw::ctrl;

RegularSwitchController::RegularSwitchController(QObject * parent) :
	BaseSwitchController(parent)
{
}

void RegularSwitchController::status(RegularSwitchController::Status & status) const
{
	BaseSwitchController::status(status);

	status.left     = isLeft();
	status.right    = isRight();
	status.inclined = isInclined();

	Q_ASSERT(status.left != status.right);
}

bool RegularSwitchController::isExpandable() const
{
	return true;
}
