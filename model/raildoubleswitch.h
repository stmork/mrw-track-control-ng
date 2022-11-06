//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAILDOUBLESWITCH_H
#define MRW_MODEL_RAILDOUBLESWITCH_H

#include <model/railpart.h>
#include <model/switchmodulereference.h>

namespace mrw::model
{
	/**
	 * This RailPart represents a double cross switch with four connectors.
	 * The naming is as follows:
	 * <pre>
	 *     A_   _C
	 *       \_/
	 *      _/ \_
	 *     B     D
	 * </pre>
	 */
	class RailDoubleSwitch :
		public RailPart,
		public SwitchModuleReference
	{
		const bool     ad_branch;
		const bool     bc_branch;

		RailPart   *   a = nullptr;
		RailPart   *   b = nullptr;
		RailPart   *   c = nullptr;
		RailPart   *   d = nullptr;

	public:
		explicit RailDoubleSwitch(
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
