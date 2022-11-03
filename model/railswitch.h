//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAILSWITCH_H
#define MRW_MODEL_RAILSWITCH_H

#include <model/assemblypart.h>
#include <model/switchmodulereference.h>
#include <model/device.h>

namespace mrw::model
{
	class SwitchModule;
	class SwitchModuleReference;

	class RailSwitch :
		public AssemblyPart,
		public Device,
		public SwitchModuleReference
	{
		const bool     left_branch;
		const bool     right_branch;
		const bool     left_prio;
		const bool     right_prio;

		AssemblyPart * a = nullptr;
		AssemblyPart * b = nullptr;
		AssemblyPart * c = nullptr;

	public:
		explicit RailSwitch(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
