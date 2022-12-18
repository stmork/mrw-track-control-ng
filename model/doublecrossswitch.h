//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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
			 * @image html XSwitch_AC_RUF.jpg width=25%
			 */
			AC = 0,

			/**
			 * @image html XSwitch_AD_RUF.jpg width=25%
			 */
			AD = D_MASK,

			/**
			 * @image html XSwitch_BC_RUF.jpg width=25%
			 */
			BC = B_MASK,

			/**
			 * @image html XSwitch_BD_RUF.jpg width=25%
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
		 * @see commandState()
		 */
		void setState(const State state);

		/**
		 * This method computes the DoubleCrossSwitch::State value depending
		 * on the given neighbour RaiPart pointers. The RailPart pointer are
		 * independent on the direction a possible Route is using.
		 *
		 * @param left The RailPart at one switch end.
		 * @param right The RailPart at the opposite end of the switch.
		 */
		void setState(const RailPart * left, const RailPart * right) override;

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

	private:
		void link() override;
		void findFlankSwitches() override;

		State                                              switch_state = State::AC;
		const static mrw::util::ConstantEnumerator<State>  state_map;
	};
}

#endif
