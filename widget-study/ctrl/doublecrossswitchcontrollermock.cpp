//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontrollermock.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

using Bending   = Position::Bending;
using LockState = Device::LockState;
using State     = DoubleCrossSwitch::State;

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

void DoubleCrossSwitchControllerMock::setLock(const LockState lock)
{
	lock_state = lock;
	emit update();
}

void DoubleCrossSwitchControllerMock::setSectionState(const SectionState state)
{
	section_state = state;
	emit update();
}

void DoubleCrossSwitchControllerMock::setSwitchState(const State state)
{
	switch_state = state;
	emit update();
}

bool DoubleCrossSwitchControllerMock::isDirection() const
{
	return !a_is_dir;
}

const QString & DoubleCrossSwitchControllerMock::name() const
{
	static const QString switch_name("819");

	return switch_name;
}

SectionState DoubleCrossSwitchControllerMock::state() const
{
	return section_state;
}

State DoubleCrossSwitchControllerMock::switchState() const
{
	return switch_state;
}

LockState DoubleCrossSwitchControllerMock::lock() const
{
	return lock_state;
}

Bending DoubleCrossSwitchControllerMock::bending() const
{
	return Bending::STRAIGHT;
}

bool DoubleCrossSwitchControllerMock::hasFlankProtection() const
{
	return flank_protection;
}

bool mrw::ctrl::DoubleCrossSwitchControllerMock::isRightBended() const
{
	return !left_handed;
}

void DoubleCrossSwitchControllerMock::setLeftHanded(const bool left)
{
	left_handed = left;
	emit computeConnectors();
	emit update();
}

void DoubleCrossSwitchControllerMock::setRightHanded(const bool right)
{
	left_handed = !right;
	emit computeConnectors();
	emit update();
}

void DoubleCrossSwitchControllerMock::setFlankProtection(const bool flank)
{
	flank_protection = flank;
	emit update();
}
