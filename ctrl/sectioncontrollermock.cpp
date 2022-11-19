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

QString SectionControllerMock::name() const
{
	return "301";
}
