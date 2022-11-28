//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/railcontrollermock.h>

using namespace mrw::model;
using namespace mrw::ctrl;

using Bending = Position::Bending;

RailControllerMock::RailControllerMock(QObject * parent) :
	RailController(parent)
{
}

void RailControllerMock::setSectionState(const SectionState state)
{
	section_state = state;
	emit update();
}

void RailControllerMock::setLock(const Device::LockState lock)
{
	lock_state = lock;
	emit update();
}

void RailControllerMock::setDirection(const bool dir)
{
	a_is_dir = !dir;
	emit computeConnectors();
	emit update();
}

void RailControllerMock::setEnds(const bool a, const bool b)
{
	a_ends = a;
	b_ends = b;
	emit computeConnectors();
	emit update();
}

void RailControllerMock::setBending(const Bending bending)
{
	bending_state = bending;
	emit computeConnectors();
	emit update();
}

QString RailControllerMock::name() const
{
	return "301";
}

void RailControllerMock::setExtension(const int input)
{
	ext_count = input;
	emit extend();
	emit computeConnectors();
	emit update();
}

void RailControllerMock::setLines(const int input)
{
	line_count = input;
	emit extend();
	emit computeConnectors();
	emit update();
}
