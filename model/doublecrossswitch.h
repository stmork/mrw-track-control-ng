//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_CROSSDOUBLESWITCH_H
#define MRW_MODEL_CROSSDOUBLESWITCH_H

#include <model/abstractswitch.h>

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
	class DoubleCrossSwitch : public AbstractSwitch
	{
		const bool     ad_branch;
		const bool     bc_branch;

		RailPart   *   a = nullptr;
		RailPart   *   b = nullptr;
		RailPart   *   c = nullptr;
		RailPart   *   d = nullptr;

	public:
		explicit DoubleCrossSwitch(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		bool    valid() const override;
		QString toString() const override;
		QString key() const override;

	private:
		void link() override;
		void findFlankSwitches() override;
	};
}

#endif
