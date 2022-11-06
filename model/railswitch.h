//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAILSWITCH_H
#define MRW_MODEL_RAILSWITCH_H

#include <model/railpart.h>
#include <model/switchmodulereference.h>

namespace mrw::model
{
	class SwitchModule;

	class RailSwitch :
		public RailPart,
		public SwitchModuleReference
	{
		const bool     left_branch;
		const bool     right_branch;
		const bool     left_prio;
		const bool     right_prio;

		RailPart   *   a = nullptr;
		RailPart   *   b = nullptr;
		RailPart   *   c = nullptr;

	public:
		explicit RailSwitch(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
		bool valid() const override;
		QString toString() const override;

		inline const QString & name() const override
		{
			return part_name;
		}
	};
}

#endif
