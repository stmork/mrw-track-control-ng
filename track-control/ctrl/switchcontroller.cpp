//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <util/method.h>
#include <statecharts/timerservice.h>
#include <ctrl/controllerregistry.h>

#include "ctrl/switchcontroller.h"

using namespace mrw::can;
using namespace mrw::statechart;
using namespace mrw::ctrl;

SwitchController::SwitchController() :
	statechart(nullptr)
{
	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);

	Q_ASSERT(statechart.check());
	statechart.enter();
}

SwitchController::~SwitchController()
{
	statechart.exit();
}

void SwitchController::inc()
{
	increase();
}

void SwitchController::dec()
{
	decrease();
}
