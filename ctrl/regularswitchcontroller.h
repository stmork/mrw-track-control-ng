//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLER_H
#define MRW_CTRL_REGULARSWITCHCONTROLLER_H

#include <ctrl/baseswitchcontroller.h>

namespace mrw::ctrl
{
	/**
	 * This class provides the logic for a simple regular switch.
	 *
	 * @see BaseController
	 * @see mrw::model::RegularSwitch
	 */
	class RegularSwitchController : public BaseSwitchController
	{
		Q_OBJECT

	public:
		/**
		 * This struct contains the complete Status of this
		 * RegularSwitchController instance including the
		 * BaseSwitchController::Status.
		 */
		struct Status : public BaseSwitchController::Status
		{
			/**
			 * True if the switch has been turned left. False otherwise.
			 *
			 * @see RegularSwitchController::isLeft()
			 */
			bool left     = false;

			/**
			 * True if the switch has been turned right. False otherwise.
			 *
			 * @see RegularSwitchController::isRight()
			 */
			bool right    = false;

			/**
			 * True if switch has to be drawn inclined.
			 *
			 * @see RegularSwitchController::isInclined()
			 */
			bool inclined = false;
		};

		explicit RegularSwitchController(QObject * parent = nullptr);

		/**
		 * This method returns the complete Status of this
		 * RegularSwitchController instance.
		 *
		 * @note This method calls the base class method
		 * BaseSwitchController::status() method.
		 *
		 * @param status The Status struct to fill.
		 */
		void status(RegularSwitchController::Status & status) const;

	private:
		virtual bool            isExpandable() const override;

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
