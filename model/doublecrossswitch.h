//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_CROSSDOUBLESWITCH_H
#define MRW_MODEL_CROSSDOUBLESWITCH_H

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

		static const unsigned B_MASK = 2;
		static const unsigned D_MASK = 1;

	public:
		enum class State : unsigned
		{
			AC = 0,
			AD = 1,
			BC = 2,
			BD = 3
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
		inline State state() const
		{
			return switch_state;
		}

		/**
		 * This method sets the internal state of this DoubleCrossSwitch. As
		 * of the DoubleCrossSwitch::State enumeration it can have four states
		 * related to a track but only two states related to its actuators.
		 *
		 * @param state The new state related to an active track.
		 * @see commandState()
		 */
		inline void setState(const State state)
		{
			switch_state = state;
		}

		bool    valid() const override;
		QString toString() const override;
		QString key() const override;
		mrw::can::SwitchState commandState() const override;

	private:
		void link() override;
		void findFlankSwitches() override;

		State switch_state = State::AC;
	};
}

#endif
