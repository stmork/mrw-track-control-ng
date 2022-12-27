//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_DOUBLECROSSSWITCHCONTROLLER_H
#define MRW_CTRL_DOUBLECROSSSWITCHCONTROLLER_H

#include <ctrl/basecontroller.h>
#include <model/doublecrossswitch.h>

namespace mrw::ctrl
{
	/**
	 * This class provides the logic for a double cross switch.
	 *
	 * @see BaseController
	 * @see mrw::model::DoubleCrossSwitch
	 */
	class DoubleCrossSwitchController : public BaseController
	{
		Q_OBJECT

	public:
		explicit DoubleCrossSwitchController(QObject * parent = nullptr);

		virtual float extensions() const override;

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
		virtual bool isRightBended() const = 0;

		/**
		 * This returns actual the mrw::model::DoubleCrossSwitch::State.
		 *
		 * @return The actual mrw::model::DoubleCrossSwitch::State.
		 */
		virtual mrw::model::DoubleCrossSwitch::State  switchState() const = 0;
	};
}

#endif
