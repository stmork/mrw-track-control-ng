//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include "collections.h"

using namespace mrw::test;
using namespace mrw::model;

using Bending   = Position::Bending;
using LockState = Device::LockState;
using Symbol    = Signal::Symbol;

const std::vector<bool> Collections::booleans
{
	true, false
};

const std::vector<Bending> Collections::bendings
{
	Bending::LEFT, Bending::STRAIGHT, Bending::RIGHT
};

const std::vector<SectionState> Collections::section_states
{
	SectionState::FREE,
	SectionState::SHUNTING,
	SectionState::TOUR,
	SectionState::OCCUPIED,
	SectionState::PASSED
};

const std::vector<LockState> Collections::lock_states
{
	LockState::FAIL,
	LockState::UNLOCKED,
	LockState::PENDING,
	LockState::LOCKED
};

const std::vector<Symbol> Collections::signal_symbols
{
	Symbol::OFF, Symbol::STOP, Symbol::GO
};
