//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#ifndef MRW_MODEL_RAIL_H
#define MRW_MODEL_RAIL_H

#include "railpart.h"

namespace mrw::model
{
	class Rail : public RailPart
	{
		const bool is_main;
		const bool is_branch;

		RailPart    *    a = nullptr;
		RailPart    *    b = nullptr;

	public:
		explicit Rail(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
