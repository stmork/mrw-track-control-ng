//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_BASESWITCHCONTROLLER_H
#define MRW_CTRL_BASESWITCHCONTROLLER_H

#include <ctrl/basecontroller.h>

namespace mrw::ctrl
{
	class BaseSwitchController : public BaseController
	{
		Q_OBJECT

	public:
		explicit BaseSwitchController(QObject * parent = nullptr);

		/**
		 * This method returns true if the switch should be drawn right handed.
		 * @note It is possible to draw a right bended switch which is in
		 * reality left handed. So you should distinguish between left/right
		 * handed against left/right <em>bended</em>.
		 *
		 * @note There is no isLeftBended() method which is the opposite of
		 * this method.
		 *
		 * @return True if the switch should be drawn right <em>bended</em>.
		 * @see mrw::model::Position::bending()
		 * @see mrw::model::Position::setBending()
		 */
		virtual bool  isRightBended() const = 0;

		virtual bool hasFlankProtection() const = 0;
	};
}

#endif
