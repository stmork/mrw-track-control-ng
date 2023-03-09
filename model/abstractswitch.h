//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_ABSTRACTSWITCH_H
#define MRW_MODEL_ABSTRACTSWITCH_H

#include <unordered_set>
#include <functional>

#include <can/commands.h>
#include <util/method.h>
#include <model/railpart.h>
#include <model/switchmodulereference.h>

namespace mrw::model
{
	class RegularSwitch;

	typedef std::function<bool(RegularSwitch *)> FlankGuard;

	/**
	 * This class represents any representation of any switch occuring on a
	 * model railway. Actually there are two switches possible:
	 * - regular switch (RegularSwitch)
	 * - double cross switch. (DoubleCrossSwitch)
	 *
	 * There is support for flank protection switches. Those switches prevents
	 * (parts of) foreign trains running into a train running onto a tour Route.
	 * So if a regular switch inside a valid Route is turned to State::AC and
	 * at connection @c b is also a RegularSwitch interconnected to connection
	 * @c b this RegularSwitch has also be turned to State::AC. Interconnected
	 * means that between both switches may be Rail elements installed but
	 * the complete interconnection has to be in the same Region.
	 *
	 * So the following rules apply to a tour Route RegularSwitch flank
	 * protection:
	 * 1. From the local connection @c b the flank protection (foreign)
	 * RegularSwitch is interconnected its connection @c b.
	 * 2. From the local connection @c c the flank protection (foreign)
	 * RegularSwitch is nterconnected its connection @c c.
	 *
	 * DoubleCrossSwitch instances may also have flank protection switches with
	 * the following rules:
	 * 1. From the local connection @c a the flank protection (foreign)
	 * RegularSwitch is interconnected its connection @c c.
	 * 2. From the local connection @c b the flank protection (foreign)
	 * RegularSwitch is interconnected its connection @c b.
	 * 3. From the local connection @c c the flank protection (foreign)
	 * RegularSwitch is interconnected its connection @c b.
	 * 4. From the local connection @c d the flank protection (foreign)
	 * RegularSwitch is interconnected its connection @c c.
	 *
	 * Flank protection switches are always RegularSwitch instances and never
	 * interconnects with connection @c a.
	 *
	 * @note: Flank protection is only applied on SectionState::TOUR Route.
	 */
	class AbstractSwitch :
		public RailPart,
		public SwitchModuleReference
	{
		friend class Section;

	protected:
		std::vector<RegularSwitch *> flank_switches;

	public:
		explicit AbstractSwitch(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);

		// Implementations from Device
		const QString    &   name()         const override;
		bool                 isUnlockable() const override;
		mrw::can::MrwMessage configMsg(const unsigned pin) const override;

		virtual void collectFlankSwitches() = 0;

		/**
		 * This method checks whether a connected AbstractSwitch is a flank
		 * protection switch.
		 *
		 * @param other The AbstractSwitch to check.
		 * @return True if the connected AbstractSwitch is a flank protection
		 * switch.
		 */
		bool isFlankProtection(const AbstractSwitch * other) const;

		/**
		 * This method collects all indirect connected switches which needs
		 * flank protection depending on the internal own state. Internally
		 * all connections which are not involved into the tour route are
		 * tested. The resulting count is the amount of correctly switched
		 * flank switches. If the result vector size differs from this result
		 * a flank switch has to be turned later. Also if the @c set_state
		 * parameter is true the result vector size and result count is
		 * equal.
		 *
		 * @param switches The collection of switches selected for flank
		 * protection.
		 * @param set_state If true the state of the flank protection switch
		 * will be set accordingly.
		 * @return The amount if correctly switched flank switches. The state is
		 * correct if all members of the collected switches have the correct
		 * mrw::can::SwitchState active.
		 * @see isFlankProtection
		 * @see follow
		 * @see linked
		 */
		virtual size_t flank(
			std::vector<RegularSwitch *> & switches,
			const bool                     set_state = false,
			FlankGuard                     guard = &mrw::util::Method::always<RegularSwitch>) const = 0;

		/**
		 * This method computes the flank protection switches using the
		 * setState() and flank() method.
		 *
		 * @param switches The collected flank protection switches.
		 * @param prev The previous RailPart in Route order.
		 * @param succ The successive RailPart in Route order.
		 * @return The amount if correctly switched flank switches. The state is
		 * correct if all members of the collected switches have the correct
		 * mrw::can::SwitchState active.
		 * @see follow()
		 * @see flank()
		 */
		virtual size_t flankCandidates(
			std::vector<RegularSwitch *> & switches,
			const RailPart        *        prev,
			const RailPart        *        succ) const = 0;

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
		 * to a RegularSwitch or indirectly connected using a Rail. The
		 * algorithm proceeds along Rail elements until a RegularSwitch is
		 * found. Note that the algorithm aborts if the RailPart leaves the
		 * originating Region.
		 *
		 * @note This is the non const version to follow Rail elements.
		 *
		 * @param part The RailPart to follow to a paired switch.
		 * @param dir The counting direction to proceed the Rail elements.
		 * @param left If true test the left branch of the potential flank
		 * switch candidate test the right branch otherwise.
		 * @return The paired RegularSwitch if any or @c nullptr.
		 * @note This method is needed to find a paired flank switch but a
		 * found RegularSwitch is only a candidate for a flank switch.
		 */
		RegularSwitch * follow(
			RailPart * part,
			const bool dir,
			const bool left) const;
	};
}

#endif
