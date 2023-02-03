//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_REGULARSWITCH_H
#define MRW_MODEL_REGULARSWITCH_H

#include <util/constantenumerator.h>
#include <model/abstractswitch.h>

namespace mrw::model
{
	class SwitchModule;

	/**
	 * This class represents a simple switch which divides a rail into two
	 * rails.
	 *
	 * A switch may be a left-hand or a right-hand switch. The corresponding
	 * branch is curved so the other divided rail may have more priority when
	 * choosing a track.
	 */
	class RegularSwitch : public AbstractSwitch
	{
		friend class DoubleCrossSwitch;

		bool         left_branch;
		bool         right_branch;
		const bool   left_prio;
		const bool   right_prio;

		RailPart  *  a = nullptr;
		RailPart  *  b = nullptr;
		RailPart  *  c = nullptr;

	public:
		enum class State : unsigned
		{
			/**
			 * The turned left RegularSwitch:
			 *
			 * <img src="RSwitch_AB_nL_RUF.jpg" width="100"/> <img src="RSwitch_AB_nR_RUF.jpg" width="100"/>
			 *
			 * In case drawing is inclined:
			 *
			 * <img src="RSwitch_AB_IL_RUF.jpg" width="100"/> <img src="RSwitch_AB_IR_RUF.jpg" width="100"/>
			 */
			AB = 1,

			/**
			 * The turned right RegularSwitch:
			 *
			 * <img src="RSwitch_AC_nL_RUF.jpg" width="100"/> <img src="RSwitch_AC_nR_RUF.jpg" width="100"/>
			 *
			 * In case drawing is inclined:
			 *
			 * <img src="RSwitch_AC_IL_RUF.jpg" width="100"/> <img src="RSwitch_AC_IR_RUF.jpg" width="100"/>
			 */
			AC = 2
		};

		explicit RegularSwitch(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);

		bool isRightHanded() const;

		bool isBranch() const override;

		/**
		 * This method returns the internal state of this RegularSwitch. As
		 * of the RegularSwitch::State enumeration it can have two states. So
		 * The active track state and the actuator state are the same.
		 *
		 * @return The internal state.
		 * @see commandState()
		 */
		State state() const;

		/**
		 * This method sets the internal state of this RegularSwitch. As
		 * of the RegularSwitch::State enumeration it can have two states. So
		 * The active track state and the actuator state are the same.
		 *
		 * @param state The new internal state.
		 * @param force Force setting the state. This is used to force
		 * update on CAN MrwMessage response.
		 * @see commandState()
		 */
		void setState(const State state, const bool force = false);

		void setState(const RailPart * left, const RailPart * right) override;

		bool                  valid()       const override;
		QString               toString()    const override;
		QString               key()         const override;
		mrw::can::SwitchState switchState() const override;

		/**
		 * This method returns the clear text QString of the State this
		 * RegularSwitch is set to.
		 *
		 * @param state The State enumeration to translate.
		 * @return The QString clear text of the given State enumeration.
		 */
		static QString get(const State & state);

		RegularSwitch * flank() const;

	private:
		void link() override;
		bool isFlankProtection(const RailPart * other) const override;

		State switch_state = State::AB;
		const static mrw::util::ConstantEnumerator<State>  state_map;
	};
}

#endif
