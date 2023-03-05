//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_CROSSDOUBLESWITCH_H
#define MRW_MODEL_CROSSDOUBLESWITCH_H

#include <util/constantenumerator.h>
#include <model/abstractswitch.h>

namespace mrw::model
{
	/**
	 * This RailPart represents a double cross switch with four connectors.
	 * The naming is as follows:
	 * <pre>
	 *     A_   _C
	 *       \_/
	 *      _/ \_
	 *     B     D
	 * </pre>
	 */
	class DoubleCrossSwitch : public AbstractSwitch
	{
		bool         ad_branch;
		bool         bc_branch;

		RailPart  *  a = nullptr;
		RailPart  *  b = nullptr;
		RailPart  *  c = nullptr;
		RailPart  *  d = nullptr;

	public:
		static const unsigned B_MASK = 2;
		static const unsigned D_MASK = 1;

		enum class State : unsigned
		{
			/**
			 * <img src="XSwitch_AC_RUF.jpg" width="100"/>
			 */
			AC = 0,

			/**
			 * <img src="XSwitch_AD_RUF.jpg" width="100"/>
			 */
			AD = D_MASK,

			/**
			 * <img src="XSwitch_BC_RUF.jpg" width="100"/>
			 */
			BC = B_MASK,

			/**
			 * <img src="XSwitch_BD_RUF.jpg" width="100"/>
			 */
			BD = B_MASK | D_MASK
		};

		explicit DoubleCrossSwitch(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);

		/**
		 * This method returns the internal state of this DoubleCrossSwitch. As
		 * of the DoubleCrossSwitch::State enumeration it can have four states
		 * related to a track but only two states related to its actuators.
		 *
		 * @return The internal state related to an active track.
		 * @see commandState()
		 */
		State state() const;

		/**
		 * This method sets the internal state of this DoubleCrossSwitch. As
		 * of the DoubleCrossSwitch::State enumeration it can have four states
		 * related to a track but only two states related to its actuators.
		 *
		 * @param state The new state related to an active track.
		 * @param force Force setting the state. This is used to force
		 * update on CAN MrwMessage response.
		 * @see commandState()
		 */
		void setState(const State state, const bool force = false);

		/**
		 * This method computes the DoubleCrossSwitch::State value depending
		 * on the given neighbour RaiPart pointers. The RailPart pointer are
		 * independent on the direction a possible Route is using.
		 *
		 * @param prev The previous RailPart in Route order.
		 * @param succ The successive RailPart in Route order.
		 */
		void setState(
			const RailPart * prev,
			const RailPart * succ) override;

		/**
		 * This method returns the clear text QString of the State this
		 * DoubleCrossSwitch is set to.
		 *
		 * @param state The State enumeration to translate.
		 * @return The QString clear text of the given State enumeration.
		 */
		static QString get(const State & state);

		bool                  valid() const override;
		QString               toString() const override;
		QString               key() const override;
		mrw::can::SwitchState switchState() const override;
		bool                  isBranch() const override;

		size_t flank(
			std::vector<RegularSwitch *> & switches,
			const bool                     set_state = false) const override;

		size_t flankCandidates(
			std::vector<RegularSwitch *> & switches,
			const RailPart        *        prev,
			const RailPart        *        succ) const override;

	private:
		void   link() override;
		State  computeState(
			const RailPart * prev,
			const RailPart * succ) const;

		bool   isFlankProtection(const AbstractSwitch * other) const override;
		size_t flank(
			std::vector<RegularSwitch *> & switches,
			const bool                     set_state,
			const State                    compare) const;
		size_t flank(
			std::vector<RegularSwitch *> & switches,
			const bool                     set_state,
			const bool                     left,
			const bool                     dir,
			RailPart           *           other) const;

		State                                              switch_state = State::AC;
		const static mrw::util::ConstantEnumerator<State>  state_map;
	};
}

#endif
