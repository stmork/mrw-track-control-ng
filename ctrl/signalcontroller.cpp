//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/signalcontroller.h>

using namespace mrw::ctrl;

SignalController::SignalController(QObject * parent) :
	BaseController(parent)
{
}

void SignalController::status(SignalController::Status & status)
{
	BaseController::status(status);

	status.main_state    = main();
	status.distant_state = distant();
	status.shunt_state   = shunt();

	status.has_main      = hasMain();
	status.has_distant   = hasDistant();
	status.has_shunting  = hasShunting();
}
