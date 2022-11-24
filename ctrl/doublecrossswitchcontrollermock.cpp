//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontrollermock.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

DoubleCrossSwitchControllerMock::DoubleCrossSwitchControllerMock(QObject * parent) :
	DoubleCrossSwitchController(parent)
{
}

void DoubleCrossSwitchControllerMock::setDirection(const bool dir)
{
	direction = dir;
	emit computeConnectors();
	emit update();
}

void DoubleCrossSwitchControllerMock::setLock(const Device::LockState lock)
{
	lock_state = lock;
	emit update();
}

void DoubleCrossSwitchControllerMock::setSectionState(const SectionState state)
{
	section_state = state;
	emit update();
}

void DoubleCrossSwitchControllerMock::setSwitchState(const DoubleCrossSwitch::State state)
{
	switch_state = state;
	emit update();
}

bool DoubleCrossSwitchControllerMock::isDirection() const
{
	return direction;
}

QString DoubleCrossSwitchControllerMock::name() const
{
	return "819";
}

Position::Curve mrw::ctrl::DoubleCrossSwitchControllerMock::curve() const
{
	return Position::Curve::STRAIGHT;
}
