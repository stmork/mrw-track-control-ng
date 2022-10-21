//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#ifndef MRW_MODEL_RAILSWITCH_H
#define MRW_MODEL_RAILSWITCH_H

#include "railpart.h"

namespace mrw::model
{
	class RailSwitch : public RailPart
	{
		const bool           left_branch;
		const bool           right_branch;
		const bool           left_prio;
		const bool           right_prio;

		RailPart    *    a = nullptr;
		RailPart    *    b = nullptr;
		RailPart    *    c = nullptr;

	public:
		explicit RailSwitch(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
