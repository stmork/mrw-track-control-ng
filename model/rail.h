//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAIL_H
#define MRW_MODEL_RAIL_H

#include <model/railpart.h>

namespace mrw::model
{
	/**
	 * This class represents a simple rail with two connectors to other
	 * Assembly Parts. In opposite to switches one connector may be not
	 * connected representing an end rail.
	 */
	class Rail : public RailPart
	{
		const bool     is_main;
		const bool     is_branch;

		RailPart   *   a = nullptr;
		RailPart   *   b = nullptr;

	public:
		explicit Rail(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;

		bool contains(const RailPart * rail) const override;
		bool valid() const override;
	};
}

#endif
