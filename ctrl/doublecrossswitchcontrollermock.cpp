//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontrollermock.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

using Bending = Position::Bending;

DoubleCrossSwitchControllerMock::DoubleCrossSwitchControllerMock(QObject * parent) :
	DoubleCrossSwitchController(parent)
{
}

void DoubleCrossSwitchControllerMock::setDirection(const bool dir)
{
	a_is_dir = !dir;
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
	return !a_is_dir;
}

QString DoubleCrossSwitchControllerMock::name() const
{
	return "819";
}

Bending DoubleCrossSwitchControllerMock::bending() const
{
	return Bending::STRAIGHT;
}
