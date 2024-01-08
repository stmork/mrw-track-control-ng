//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <util/method.h>
#include <statecharts/timerservice.h>

#include "ctrl/controllerregistry.h"
#include "ctrl/switchcontroller.h"

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::statechart;
using namespace mrw::ctrl;

SwitchController::SwitchController() : SelfPointer<OperationCallback>(this)
{
	statechart.setTimerService(TimerService::instance());
	statechart.setOperationCallback(*this);

	Q_ASSERT(statechart.check());
	statechart.enter();
}

SwitchController::~SwitchController()
{
	statechart.exit();
}

void SwitchController::setManual(const bool manual)
{
	statechart.setIsManual(manual);
}

void SwitchController::inc()
{
	increase();
}

void SwitchController::dec()
{
	decrease();
}

bool SwitchController::isReserved()
{
	return railPart()->reserved();
}

bool SwitchController::isFlankRelevantReserved() const
{
	return section()->isFlankRelevant() && railPart()->reserved();
}
