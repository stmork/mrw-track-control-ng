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
		const bool     ad_branch;
		const bool     bc_branch;

		RailPart   *   a = nullptr;
		RailPart   *   b = nullptr;
		RailPart   *   c = nullptr;
		RailPart   *   d = nullptr;

	public:
		static const unsigned B_MASK = 2;
		static const unsigned D_MASK = 1;

		enum class State : unsigned
		{
			AC = 0,
			AD = D_MASK,
			BC = B_MASK,
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

		void setState(const RailPart * left, const RailPart * right) override;

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
