//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/regularswitchcontrollermock.h>

using namespace mrw::model;
using namespace mrw::ctrl;

RegularSwitchControllerMock::RegularSwitchControllerMock(QObject * parent) :
	RegularSwitchController(parent)
{
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
	emit update();
}

void RegularSwitchControllerMock::setRightHanded(const bool right)
{
	left_handed = !right;
	emit update();
}

void RegularSwitchControllerMock::setDirection(const bool dir)
{
	direction = dir;
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

bool RegularSwitchControllerMock::isRightHanded() const
{
	return !left_handed;
}

bool RegularSwitchControllerMock::isDirection() const
{
	return direction;
}

QString RegularSwitchControllerMock::name() const
{
	return "819";
}
