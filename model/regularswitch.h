//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_REGULARSWITCH_H
#define MRW_MODEL_REGULARSWITCH_H

#include <can/commands.h>
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

		const bool     left_branch;
		const bool     right_branch;
		const bool     left_prio;
		const bool     right_prio;

		RailPart   *   a = nullptr;
		RailPart   *   b = nullptr;
		RailPart   *   c = nullptr;

		mrw::can::SwitchState  switch_state = mrw::can::SwitchState::SWITCH_STATE_LEFT;

	public:
		explicit RegularSwitch(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);

		inline bool isRightHanded() const
		{
			return right_branch;
		}

		inline mrw::can::SwitchState state() const
		{
			return switch_state;
		}

		bool    valid() const override;
		QString toString() const override;
		QString key() const override;

	private:
		void link() override;
		void findFlankSwitches() override;
	};
}

#endif
