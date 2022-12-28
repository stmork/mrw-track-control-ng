//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLER_H
#define MRW_CTRL_REGULARSWITCHCONTROLLER_H

#include <ctrl/basecontroller.h>

namespace mrw::ctrl
{
	/**
	 * This class provides the logic for a simple regular switch.
	 *
	 * @see BaseController
	 * @see mrw::model::RegularSwitch
	 */
	class RegularSwitchController : public BaseController
	{
		Q_OBJECT

	public:
		explicit RegularSwitchController(QObject * parent = nullptr);

		/**
		 * This method returns true if the switch has been turned left.
		 *
		 * @return True if the switch has been turned left. False otherwise.
		 * @see isRight()
		 */
		virtual bool  isLeft() const = 0;

		/**
		 * This method returns true if the switch has been turned right.
		 *
		 * @return True if the switch has been turned right. False otherwise.
		 * @see isLeft()
		 */
		virtual bool  isRight() const = 0;

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

		/**
		 * This method returns true if the switch should be drawn inclined.
		 *
		 * @return True if switch has to be drawn inclined.
		 * @see mrw::model::Position::isInclined()
		 * @see mrw::model::Position::toggleInclination()
		 *
		 * @image html RSwitch_AB_IL_RUF.jpg width=50
		 * @image html RSwitch_AB_IR_RUF.jpg width=50
		 */
		virtual bool  isInclined() const = 0;
	};
}

#endif
