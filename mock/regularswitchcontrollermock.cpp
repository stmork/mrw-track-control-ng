//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <mock/regularswitchcontrollermock.h>

using namespace mrw::model;
using namespace mrw::ctrl;

using Bending   = Position::Bending;
using LockState = Device::LockState;

RegularSwitchControllerMock::RegularSwitchControllerMock(QObject * parent) :
	RegularSwitchController(parent)
{
}

void RegularSwitchControllerMock::reset()
{
	setDirection();
	setExtension(0);
	setSectionState(SectionState::FREE);
	setLock(LockState::UNLOCKED);
	setFlankProtection(false);
	setInclined(false);
	setLeftHanded();
	setRight();
}

void RegularSwitchControllerMock::setLeft(const bool left)
{
	switch_state = left ? RegularSwitch::State::AB : RegularSwitch::State::AC;
	emit update();
}

void RegularSwitchControllerMock::setRight(const bool right)
{
	switch_state = right ? RegularSwitch::State::AC : RegularSwitch::State::AB;
	emit update();
}

void RegularSwitchControllerMock::setLeftHanded(const bool left)
{
	left_handed = left;
	emit computeConnectors();
	emit update();
}

void RegularSwitchControllerMock::setRightHanded(const bool right)
{
	left_handed = !right;
	emit computeConnectors();
	emit update();
}

void RegularSwitchControllerMock::setInclined(const bool incl)
{
	inclined = incl;
	emit computeConnectors();
	emit update();
}

void RegularSwitchControllerMock::setExtension(const int input)
{
	extension = input;
	emit extend();
	emit computeConnectors();
	emit update();
}

void RegularSwitchControllerMock::setFlankProtection(const bool flank)
{
	flank_protection = flank;
	emit update();
}

void RegularSwitchControllerMock::setDirection(const bool dir)
{
	a_is_dir = !dir;
	emit computeConnectors();
	emit update();
}

void RegularSwitchControllerMock::setLock(const Device::LockState lock)
{
	lock_state = lock;
	emit update();
}

void RegularSwitchControllerMock::setSectionState(const SectionState state)
{
	section_state = state;
	emit update();
}

bool RegularSwitchControllerMock::isLeft() const
{
	return switch_state == RegularSwitch::State::AB;
}

bool RegularSwitchControllerMock::isRight() const
{
	return switch_state == RegularSwitch::State::AC;
}

bool RegularSwitchControllerMock::isRightBended() const
{
	return !left_handed;
}

bool RegularSwitchControllerMock::isDirection() const
{
	return !a_is_dir;
}

const QString & RegularSwitchControllerMock::name() const
{
	static const QString switch_name("819");

	return switch_name;
}

float RegularSwitchControllerMock::extensions() const
{
	return extension;
}

Bending RegularSwitchControllerMock::bending() const
{
	return isRightBended() ?  Bending::RIGHT : Bending::LEFT;
}

bool RegularSwitchControllerMock::hasFlankProtection() const
{
	return flank_protection;
}

SectionState RegularSwitchControllerMock::state() const
{
	return section_state;
}

Device::LockState RegularSwitchControllerMock::lock() const
{
	return lock_state;
}

bool RegularSwitchControllerMock::isInclined() const
{
	return inclined;
}
