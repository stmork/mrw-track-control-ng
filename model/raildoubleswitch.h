//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAILDOUBLESWITCH_H
#define MRW_MODEL_RAILDOUBLESWITCH_H

#include "model/assemblypart.h"
#include "model/switchmodulereference.h"

namespace mrw::model
{
	class RailDoubleSwitch : public AssemblyPart, public SwitchModuleReference
	{
		const bool     ad_branch;
		const bool     bc_branch;

		AssemblyPart * a = nullptr;
		AssemblyPart * b = nullptr;
		AssemblyPart * c = nullptr;
		AssemblyPart * d = nullptr;

	public:
		explicit RailDoubleSwitch(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
