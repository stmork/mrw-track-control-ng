//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_COLLECTIONS_H
#define MRW_TEST_COLLECTIONS_H

#include <vector>

#include <model/device.h>
#include <model/section.h>
#include <model/signal.h>

namespace mrw::test
{
	class Collections
	{
	public:
		static const std::vector<bool>                          booleans;
		static const std::vector<mrw::model::Position::Bending> bendings;
		static const std::vector<mrw::model::Device::LockState> lock_states;
		static const std::vector<mrw::model::SectionState>      section_states;
		static const std::vector<mrw::model::Signal::Symbol>    signal_symbols;
	};
}

#endif
