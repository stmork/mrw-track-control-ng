//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
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
		friend class AbstractSwitch;

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
			AC = 2,

			/**
			 * This state occurs if a switch is not in defined final location
			 * due to a hardware failure.
			 */
			FAIL
		};

		explicit RegularSwitch(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);

		bool isCurved() const noexcept override;

		/**
		 * This method returns the internal state of this RegularSwitch. As
		 * of the RegularSwitch::State enumeration it can have two states. So
		 * The active track state and the actuator state are the same.
		 *
		 * @return The internal state.
		 * @see commandState()
		 */
		State state() const noexcept;

		/**
		 * This method sets the internal state of this RegularSwitch. As
		 * of the RegularSwitch::State enumeration it can have two states. So
		 * The active track state and the actuator state are the same.
		 *
		 * @param state The new internal state.
		 * @param force Force setting the state. This is used to force
		 * update on CAN MrwMessage response.
		 * @return True if the state may be changed depending on a set
		 * LockState::LOCKED.
		 * @see commandState()
		 */
		bool setState(const State state, const bool force = false) noexcept;

		[[nodiscard]]
		bool isSwitchable(
			const RailPart * prev,
			const RailPart * succ) const noexcept override;

		/**
		 * This method computes the RegularSwitch::State value depending on the
		 * given neighbour RaiPart pointers. The RailPart pointer are
		 * independent on the direction a possible Route is using.
		 *
		 * @param prev The previous RailPart in Route order.
		 * @param succ The successive RailPart in Route order.
		 * @return True if the state may be changed depending on a set
		 * LockState::LOCKED.
		 */
		bool setState(
			const RailPart * prev,
			const RailPart * succ) override;

		bool                  valid()       const noexcept override;
		QString               toString()    const noexcept override;
		QString               key()         const noexcept override;
		mrw::can::SwitchState switchState() const noexcept override;

		size_t flank(
			std::vector<RegularSwitch *> & switches,
			const bool                     set_state = false) const noexcept override;

		size_t flankCandidates(
			std::vector<RegularSwitch *> & switches,
			const RailPart        *        prev,
			const RailPart        *        succ) const noexcept override;

		/**
		 * This method returns the clear text QString of the State this
		 * RegularSwitch is set to.
		 *
		 * @param state The State enumeration to translate.
		 * @return The QString clear text of the given State enumeration.
		 */
		static QString get(const State & state);

	private:
		void   link() override;

		State  computeState(
			const RailPart * prev,
			const RailPart * succ) const;

		size_t flank(
			std::vector<RegularSwitch *> & switches,
			const bool                     set_state,
			const State                    compare) const noexcept;

		void collectFlankSwitches() noexcept override;

		State switch_state = State::AB;
		const static mrw::util::ConstantEnumerator<State>  state_map;
	};
}

#endif
