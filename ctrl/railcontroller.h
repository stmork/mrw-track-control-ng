//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_RAILCONTROLLER_H
#define MRW_CTRL_RAILCONTROLLER_H

#include <ctrl/basecontroller.h>

namespace mrw::ctrl
{
	/**
	 * This class provides the logic for a simple rail. The "logic" contains
	 * information about drawing a rail bended or ended.
	 */
	class RailController: public BaseController
	{
		Q_OBJECT

	public:
		explicit RailController(QObject * parent = nullptr);

		/**
		 * The @c a connector does not have any further RailPart and ends here.
		 *
		 * @note Only one of the methods aEnds() and bEnds() may return true.
		 *
		 * @return True if the Rail ends here.
		 */
		virtual bool  aEnds() const = 0;

		/**
		 * The @c b connector does not have any further RailPart and ends here.
		 *
		 * @note Only one of the methods aEnds() and bEnds() may return true.
		 *
		 * @return True if the Rail ends here.
		 */
		virtual bool  bEnds() const = 0;
	};
}

#endif
