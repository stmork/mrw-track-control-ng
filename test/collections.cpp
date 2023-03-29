//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "collections.h"

using namespace mrw::test;
using namespace mrw::model;

using Bending   = Position::Bending;
using LockState = Device::LockState;
using Symbol    = Signal::Symbol;

const std::vector<bool> booleans
{
	true, false
};

const std::vector<Bending> bendings
{
	Bending::LEFT, Bending::STRAIGHT, Bending::RIGHT
};

const std::vector<mrw::model::SectionState> section_states
{
	FREE, SHUNTING, TOUR, OCCUPIED, PASSED
};

const std::vector<LockState> lock_states
{
	LockState::FAIL, LockState::UNLOCKED, LockState::PENDING, LockState::LOCKED
};

const std::vector<mrw::model::Signal::Symbol> signal_symbols
{
	Symbol::OFF, Symbol::STOP, Symbol::GO
};
