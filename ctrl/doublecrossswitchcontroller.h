//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_DOUBLECROSSSWITCHCONTROLLER_H
#define MRW_CTRL_DOUBLECROSSSWITCHCONTROLLER_H

#include <ctrl/baseswitchcontroller.h>
#include <model/doublecrossswitch.h>

namespace mrw::ctrl
{
	/**
	 * This class provides the logic for a double cross switch.
	 *
	 * @see BaseController
	 * @see mrw::model::DoubleCrossSwitch
	 */
	class DoubleCrossSwitchController : public BaseSwitchController
	{
		Q_OBJECT

	public:
		/**
		 * This struct contains the complete Status of this
		 * DoubleCrossSwitchController instance including the
		 * BaseSwitchController::Status.
		 */
		struct Status : public BaseSwitchController::Status
		{
			/**
			 * The actual mrw::model::DoubleCrossSwitch::State.
			 *
			 * @see DoubleCrossSwitchController::switchState()
			 */
			mrw::model::DoubleCrossSwitch::State state;

			/**
			 * True if the a connector is involeved into a track so the
			 * DoubleCrossSwitch::State is DoubleCrossSwitch::State::AC or
			 * DoubleCrossSwitch::State::AD;
			 */
			unsigned                             is_a = false;

			/**
			 * True if the b connector is involeved into a track so the
			 * DoubleCrossSwitch::State is DoubleCrossSwitch::State::BC or
			 * DoubleCrossSwitch::State::BD;
			 */
			unsigned                             is_b = false;

			/**
			 * True if the c connector is involeved into a track so the
			 * DoubleCrossSwitch::State is DoubleCrossSwitch::State::AC or
			 * DoubleCrossSwitch::State::BC;
			 */
			unsigned                             is_c = false;

			/**
			 * True if the d connector is involeved into a track so the
			 * DoubleCrossSwitch::State is DoubleCrossSwitch::State::AD or
			 * DoubleCrossSwitch::State::BD;
			 */
			unsigned                             is_d = false;
		};

		explicit DoubleCrossSwitchController(QObject * parent = nullptr);

		/**
		 * This method returns the complete Status of this
		 * DoubleCrossSwitchController instance.
		 *
		 * @note This method calls the base class method
		 * BaseSwitchController::status() method.
		 *
		 * @param status The Status struct to fill.
		 */
		void status(DoubleCrossSwitchController::Status & status) const;

	private:

		virtual float extensions() const override;

		/**
		 * This returns actual the mrw::model::DoubleCrossSwitch::State.
		 *
		 * @return The actual mrw::model::DoubleCrossSwitch::State.
		 */
		virtual mrw::model::DoubleCrossSwitch::State  switchState() const = 0;
	};
}

#endif
