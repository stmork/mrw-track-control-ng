//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <mock/railcontrollermock.h>

using namespace mrw::model;
using namespace mrw::ctrl;

using Bending   = Position::Bending;
using LockState = Device::LockState;

RailControllerMock::RailControllerMock(QObject * parent) :
	RailController(parent)
{
}

void RailControllerMock::reset()
{
	setDirection();
	setExpansion(0);
	setLines(0);
	setSectionState(SectionState::FREE);
	setLock(LockState::UNLOCKED);
	setBending(Bending::STRAIGHT);
	setEnds(false, false);
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

const QString & RailControllerMock::name() const
{
	static const QString rail_name("301");

	return rail_name;
}

SectionState RailControllerMock::state() const
{
	return section_state;
}

Device::LockState RailControllerMock::lock() const
{
	return lock_state;
}

bool RailControllerMock::isDirection() const
{
	return !a_is_dir;
}

bool RailControllerMock::aEnds() const
{
	return a_ends;
}

bool RailControllerMock::bEnds() const
{
	return b_ends;
}

bool RailControllerMock::hasCrossing() const
{
	return crossing;
}

float RailControllerMock::expansion() const
{
	return exp_count;
}

float RailControllerMock::lines() const
{
	return line_count;
}

Position::Bending RailControllerMock::bending() const
{
	return bending_state;
}

void RailControllerMock::setExpansion(const int input)
{
	exp_count = input;
	emit expand();
	emit computeConnectors();
	emit update();
}

void RailControllerMock::setLines(const int input)
{
	line_count = input;
	emit expand();
	emit computeConnectors();
	emit update();
}

void RailControllerMock::setCrossing(const bool new_crossing)
{
	crossing = new_crossing;
	emit update();
}
