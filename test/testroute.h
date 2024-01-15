//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTROUTE_H
#define MRW_TEST_TESTROUTE_H

#include <model/route.h>
#include <model/section.h>
#include <model/railpart.h>

namespace mrw::test
{
	class TestRoute : public mrw::model::Route
	{
	public:
		explicit TestRoute(
			const bool                       dir,
			const mrw::model::SectionState   wanted_state,
			mrw::model::RailPart      *      first) :
			Route(dir, wanted_state, first)
		{
		}

		const std::vector<mrw::model::RegularSwitch *> & doFlank()
		{
			prepareFlank();
			return flank_switches;
		}
	};
}

#endif
