//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/sectioncontrollermock.h>

using namespace mrw::model;
using namespace mrw::ctrl;

using Bending = Position::Bending;

SectionControllerMock::SectionControllerMock(QObject * parent) :
	SectionController(parent)
{
}

void SectionControllerMock::setSectionState(const SectionState state)
{
	section_state = state;
	emit update();
}

void mrw::ctrl::SectionControllerMock::setLock(const Device::LockState lock)
{
	lock_state = lock;
	emit update();
}

void SectionControllerMock::setDirection(const bool dir)
{
	a_is_dir = !dir;
	emit computeConnectors();
	emit update();
}

void SectionControllerMock::setEnds(const bool a, const bool b)
{
	a_ends = a;
	b_ends = b;
	emit update();
}

void SectionControllerMock::setBending(const Bending bending)
{
	bending_state = bending;
	emit computeConnectors();
	emit update();
}

QString SectionControllerMock::name() const
{
	return "301";
}

void SectionControllerMock::setExtension(const int new_extension)
{
	extension = new_extension;
	emit extend();
	emit computeConnectors();
	emit update();
}
