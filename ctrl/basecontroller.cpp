//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include "ctrl/basecontroller.h"

using namespace mrw::ctrl;

Q_LOGGING_CATEGORY(mrw::ctrl::log, "mrw.ctrl")

BaseController::BaseController(QObject * parent) : QObject(parent)
{
}

void BaseController::status(BaseController::Status & status) const
{
	status.name          = name();
	status.position      = position();
	status.extensions    = extensions();
	status.lines         = lines();
	status.section_state = state();
	status.bending       = bending();
	status.lock_state    = lock();
	status.direction     = isDirection();
	status.expandable    = isExpandable();
}
