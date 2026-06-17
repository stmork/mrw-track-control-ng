//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
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
	 * <img src="Rail____AS__RUF.jpg" width="100"/> <img src="Rail____BS__RUF.jpg" width="100"/>
	 *
	 * Additionally the Rail may have extension in width and may be bended left:
	 *
	 * <img src="Rail____IL__RUF.jpg" width="200"/>
	 *
	 * or bended right:
	 *
	 * <img src="Rail____IR__RUF.jpg" width="200"/>
	 *
	 * @note There may exist a crossing through the rail and is drawn if the
	 * extension is at least of 4. In case the rail/section is free:
	 *
	 * <img src="Rail____IS_XRUF.jpg" width="200"/>
	 *
	 * or actually closing:
	 *
	 * <img src="Rail____IS_XRPT.jpg" width="200"/>
	 *
	 * and finally locked (e.g. tour):
	 *
	 * <img src="Rail____IS_XRUT.jpg" width="200"/>
	 */
	class Rail : public RailPart
	{
		RailPart   *   a = nullptr;
		RailPart   *   b = nullptr;

		const bool     is_main;
		const bool     is_curve;

	public:
		explicit Rail(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);

		bool        valid()    const noexcept override;
		bool        isCurved() const noexcept override;
		static bool isMain(const Rail * rail) noexcept;

		/**
		 * This method returns the rail name.
		 *
		 * @return The rail name.
		 */
		const QString & name() const noexcept;

		QString toString() const noexcept override;
		QString key()      const noexcept override;
		bool    setState(const RailPart * prev, const RailPart * succ) noexcept override;

	private:
		void link() noexcept override;
	};
}

#endif
