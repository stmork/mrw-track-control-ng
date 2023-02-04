//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_ABSTRACTSWITCH_H
#define MRW_MODEL_ABSTRACTSWITCH_H

#include <unordered_set>

#include <can/commands.h>
#include <model/railpart.h>
#include <model/switchmodulereference.h>

namespace mrw::model
{
	class RegularSwitch;

	/**
	 * This class represents any representation of any switch occuring on a
	 * model railway. Actually there are two switches possible:
	 * - regular switch (RegularSwitch)
	 * - double cross switch. (DoubleCrossSwitch)
	 */
	class AbstractSwitch :
		public RailPart,
		public SwitchModuleReference
	{
		friend class Section;

	public:
		explicit AbstractSwitch(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);

		// Implementations from Device
		const QString    &   name()         const override;
		bool                 isUnlockable() const override;
		mrw::can::MrwMessage configMsg(const unsigned pin) const override;

		/**
		 * This method checks whether a connected AbstractSwitch is a flank
		 * protection switch.
		 *
		 * @param other The AbstractSwitch to check.
		 * @return True if the connected AbstractSwitch is a flank protection
		 * switch.
		 */
		virtual bool isFlankProtection(const RailPart * other) const = 0;

		/**
		 * This method collects all indirect connected switches which needs
		 * flank protection depending on the internal own state. Internally
		 * all connection which are not involved into the tour route are
		 * tested.
		 *
		 * @param switches The collection of switches selected for flank
		 * protection.
		 * @param set_state If true the state of the flank protection switch
		 * will be set accordingly.
		 * @return The amount if correctly switched switches. The state is
		 * correct if all members of the collected switches have the correct
		 * mrw::can::SwitchState active.
		 * @see isFlankProtection
		 * @see follow
		 * @see linked
		 */
		virtual size_t flank(
			std::vector<RegularSwitch *> & switches,
			const bool                     set_state = false) const = 0;

		/**
		 * This method converts the internal switch state into the right
		 * token for a MrwMessage;
		 *
		 * @return The turn command for a MrwMessage.
		 */
		virtual mrw::can::SwitchState switchState() const = 0;

		/**
		 * This method returns the CAN command corresponding to the internal
		 * switch State.
		 *
		 * @return The corresponing CAN mrw::can::Command to reach the state.
		 * @see mrw::can::Command
		 */
		mrw::can::Command commandState() const;

	protected:
		/**
		 * This method follows a connection part if it is directly connected
		 * to a RegularSwitch or indirectly connected using a Rail.
		 *
		 * @param part The RailPart to follow to a paired switch.
		 * @return The paired RegularSwitch if any or @c nullptr.
		 * @note This method is needed to find a paired flank switch but a
		 * found RegularSwitch is only a candidate for a flank switch.
		 */
		RegularSwitch * follow(RailPart * part) const;

		/**
		 * This method determines if the given candidate for a paired flank
		 * switch is really linked to the AbstractSwitch to test to.
		 *
		 * @param candidate The flank switch candidate to test. This is a non
		 * @c nullptr result from the follow() method.
		 * @param self The local flank switch pair.
		 * @return True if the candidate and self parameter are paired
		 * flank switches.
		 * @see follow()
		 */
		bool linked(
			const RailPart    *    candidate,
			const AbstractSwitch * self) const;
	};
}

#endif
