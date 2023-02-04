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
	/**
	 * This class provides virtual pure methods which are common to the
	 * RegularSwitchController and the DoubleCrossSwitchController.
	 */
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

		/**
		 * This method computes if any flank protection is active for this
		 * switch. There are several rules which must all comply:
		 * 1. The mrw::model::Device::LockState is mrw::model::Device::LockState::LOCKED.
		 * 2. The mrw::model::SectionState is mrw::model::SectionState::TOUR.
		 * 3. All flank protection switch canditates are in the
		 * correct mrw::can::SwitchState using the
		 * mrw::model::AbstractSwitch::flank() method.
		 *
		 * The central part of the computation is the
		 * mrw::model::AbstractSwitch::flank() method. This method returns the
		 * flank switch candidates and checks whether they are all in the
		 * correct switching state.
		 *
		 * The regular switch will appear as follows for a free, a shunting
		 * route and a tour route with an indicated flank protection.
		 *
		 * <img src="RSwitch_AC_nL_RUF.jpg" width="100"/>
		 * <img src="RSwitch_AC_nL_RLS.jpg" width="100"/>
		 * <img src="RSwitch_AC_nL_RLTP.jpg" width="100"/>
		 *
		 * The double cross switch will appear as follows for a free, a
		 * shunting route and a tour route with an indicated flank protection.
		 *
		 * <img src="XSwitch_AD_RUF.jpg" width="100"/>
		 * <img src="XSwitch_AD_RLS.jpg" width="100"/>
		 * <img src="XSwitch_AD_RLTP.jpg" width="100"/>
		 *
		 * @return True if flank protection is assured.
		 * @see mrw::model::AbstractSwitch::flank()
		 */
		virtual bool hasFlankProtection() const = 0;
	};
}

#endif
