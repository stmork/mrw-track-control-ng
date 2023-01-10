//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAIL_H
#define MRW_MODEL_RAIL_H

#include <model/railpart.h>

namespace mrw::model
{
	/**
	 * This class represents a simple rail with two connectors to other
	 * RailParts. In opposite to switches one and only one connector may be not
	 * connected representing an end rail.
	 *
	 * <img src="Rail____AS_RUF.jpg" width="100"/> <img src="Rail____BS_RUF.jpg" width="100"/>
	 *
	 * Additionally the Rail may have extension in width and may be bended left:
	 *
	 * <img src="Rail____IL_RUF.jpg" width="200"/>
	 *
	 * or bended right:
	 *
	 * <img src="Rail____IR_RUF.jpg" width="200"/>
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
			Section       *       model_section,
			const QDomElement  &  element);

		bool        valid() const override;
		bool        isBranch() const override;
		static bool isMain(const Rail * rail);

		/**
		 * This method returns the rail name.
		 *
		 * @return The rail name.
		 */
		const QString & name() const;

		QString toString() const override;
		QString key() const override;
		void setState(const RailPart * left, const RailPart * right) override;

	private:
		void link() override;
	};
}

#endif
