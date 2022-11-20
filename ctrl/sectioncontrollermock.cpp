//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/sectioncontrollermock.h>

using namespace mrw::ctrl;

SectionControllerMock::SectionControllerMock(QObject * parent) :
	SectionController(parent)
{
}

void SectionControllerMock::setSectionState(const mrw::model::SectionState state)
{
	section_state = state;
	emit update();
}

void SectionControllerMock::setDirection(const bool dir)
{
	direction = dir;
	emit update();
}

void SectionControllerMock::setEnds(const bool a, const bool b)
{
	a_ends = a;
	b_ends = b;
	emit update();
}

QString SectionControllerMock::name() const
{
	return "301";
}
