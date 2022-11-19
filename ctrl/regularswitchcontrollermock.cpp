//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/regularswitchcontrollermock.h>

using namespace mrw::can;
using namespace mrw::ctrl;

RegularSwitchControllerMock::RegularSwitchControllerMock(QObject * parent) :
	RegularSwitchController(parent)
{
}

void RegularSwitchControllerMock::setLeft(const bool left)
{
	switch_state = left ? SWITCH_STATE_LEFT : SWITCH_STATE_RIGHT;
	emit update();
}

void RegularSwitchControllerMock::setRight(const bool right)
{
	switch_state = right ? SWITCH_STATE_RIGHT : SWITCH_STATE_LEFT;
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

void RegularSwitchControllerMock::setSectionState(const mrw::model::SectionState state)
{
	section_state = state;
	emit update();
}

bool RegularSwitchControllerMock::isLeft() const
{
	return switch_state == SWITCH_STATE_LEFT;
}

bool RegularSwitchControllerMock::isRight() const
{
	return switch_state == SWITCH_STATE_RIGHT;
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
