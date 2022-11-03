//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAIL_H
#define MRW_MODEL_RAIL_H

#include "assemblypart.h"

namespace mrw::model
{
	class Rail : public AssemblyPart
	{
		const bool     is_main;
		const bool     is_branch;

		AssemblyPart * a = nullptr;
		AssemblyPart * b = nullptr;

	public:
		explicit Rail(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
