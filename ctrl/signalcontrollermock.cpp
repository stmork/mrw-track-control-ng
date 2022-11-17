//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "signalcontrollermock.h"

using namespace mrw::ctrl;

SignalControllerMock::SignalControllerMock()
{
}

void SignalControllerMock::setShunting(const bool shunt)
{
	shunting = shunt;
}

void SignalControllerMock::setDistant(const bool distant)
{
	distant_signal = distant;
}

void SignalControllerMock::setMain(const bool main)
{
	main_signal = main;
}

void SignalControllerMock::setDirection(const bool dir)
{
	direction = dir;
}

bool SignalControllerMock::isDirection() const
{
	return direction;
}

bool SignalControllerMock::hasShunting() const
{
	return shunting;
}

bool SignalControllerMock::hasDistant() const
{
	return distant_signal || (!hasMain());
}

bool SignalControllerMock::hasMain() const
{
	return main_signal;
}

QString SignalControllerMock::name() const
{
	return isDirection() ? "N3" : "P3";
}
