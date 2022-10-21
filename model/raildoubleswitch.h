//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#ifndef MRW_MODEL_RAILDOUBLESWITCH_H
#define MRW_MODEL_RAILDOUBLESWITCH_H

#include "railpart.h"

namespace mrw::model
{
	class RailDoubleSwitch : public RailPart
	{
		const bool ad_branch;
		const bool bc_branch;

		RailPart    *    a = nullptr;
		RailPart    *    b = nullptr;
		RailPart    *    c = nullptr;
		RailPart    *    d = nullptr;

	public:
		explicit RailDoubleSwitch(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
