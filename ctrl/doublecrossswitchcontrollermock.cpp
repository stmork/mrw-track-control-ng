//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/doublecrossswitchcontrollermock.h>

using namespace mrw::can;
using namespace mrw::ctrl;

DoubleCrossSwitchControllerMock::DoubleCrossSwitchControllerMock(QObject * parent) :
	DoubleCrossSwitchController(parent)
{
}

void DoubleCrossSwitchControllerMock::setDirection(const bool dir)
{
	direction = dir;
	emit update();
}

void DoubleCrossSwitchControllerMock::setSectionState(const mrw::model::SectionState state)
{
	section_state = state;
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
