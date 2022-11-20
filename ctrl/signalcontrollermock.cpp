//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/signalcontrollermock.h>

using namespace mrw::ctrl;

SignalControllerMock::SignalControllerMock(QObject * parent) :
	SignalController(parent)
{
}

void SignalControllerMock::setShunting(const bool shunt)
{
	shunting = shunt;
	emit update();
}

void SignalControllerMock::setDistant(const bool distant)
{
	distant_signal = distant;
	emit update();
}

void SignalControllerMock::setMain(const bool main)
{
	main_signal = main;
	emit update();
}

void SignalControllerMock::setDirection(const bool dir)
{
	direction = dir;
	emit update();
}

void SignalControllerMock::setSectionState(const mrw::model::SectionState state)
{
	section_state = state;
	emit update();
}

void mrw::ctrl::SignalControllerMock::setShuntStop()
{
	shunt_state = TourState::STOP;
	emit update();
}

void mrw::ctrl::SignalControllerMock::setShuntGo()
{
	shunt_state = TourState::GO;
	emit update();
}

void mrw::ctrl::SignalControllerMock::setDistantStop()
{
	distant_state = TourState::STOP;
	emit update();
}

void mrw::ctrl::SignalControllerMock::setDistantGo()
{
	distant_state = TourState::GO;
	emit update();
}

void mrw::ctrl::SignalControllerMock::setMainStop()
{
	main_state = TourState::STOP;
	emit update();
}

void mrw::ctrl::SignalControllerMock::setMainGo()
{
	main_state = TourState::GO;
	emit update();
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
	return distant_signal;
}

bool SignalControllerMock::hasMain() const
{
	return main_signal;
}

QString SignalControllerMock::name() const
{
	return isDirection() ? "N3" : "P3";
}