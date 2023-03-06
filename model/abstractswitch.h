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
		virtual bool isFlankProtection(const AbstractSwitch * other) const = 0;

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
		 * algorithm proceeds as long as a Rail is found. If there are more
		 * than MAX_FOLLOW_RAIL Rail elements to follow the method returns
		 * false.
		 *
		 * @note This is the non const version to follow Rail elements.
		 *
		 * @param part The RailPart to follow to a paired switch.
		 * @param dir The counting direction to proceed the Rail elements.
		 * @return The paired RegularSwitch if any or @c nullptr.
		 * @note This method is needed to find a paired flank switch but a
		 * found RegularSwitch is only a candidate for a flank switch.
		 * @see MAX_FOLLOW_RAIL
		 */
		static RegularSwitch * follow(RailPart * part, const bool dir);

		/**
		 * This method follows a connection part if it is directly connected
		 * to a RegularSwitch or indirectly connected using a Rail. The
		 * algorithm proceeds as long as a Rail is found. If there are more
		 * than MAX_FOLLOW_RAIL Rail elements to follow the method returns
		 * false.
		 *
		 * @note This is the const version to follow Rail elements.
		 *
		 * @param part The RailPart to follow to a paired switch.
		 * @param dir The counting direction to proceed the Rail elements.
		 * @return The paired RegularSwitch if any or @c nullptr.
		 * @note This method is needed to find a paired flank switch but a
		 * found AbstractSwitch is only a candidate for a flank switch.
		 * @see MAX_FOLLOW_RAIL
		 */
		static const AbstractSwitch * follow(
			const RailPart * part,
			const bool       dir);

		/**
		 * This method tests a connected neighbour if the neighbour is a
		 * flank protection switch. This switch may be indirectly connected
		 * via Rail elements but only Rail elements.
		 *
		 * @param other The RailPart candidate to test if it is a flank
		 * protection switch.
		 * @param left If true test the left branch of the potential flank
		 * switch candidate test the right branch otherwise.
		 * @return True if a flank protection switch is connected.
		 */
		bool isFlankCandidate(
			const RegularSwitch * other,
			const bool            left) const;

		/**
		 * This method determines if the given candidate for a paired flank
		 * switch is really linked to the AbstractSwitch to test to.
		 *
		 * @param part The RailPart to follow and test. This is a non
		 * @c nullptr result from the follow() method.
		 * @param self The local flank switch pair.
		 * @return True if the candidate and self parameter are paired
		 * flank switches.
		 * @see follow()
		 */
		bool linked(
			const RailPart    *    part,
			const AbstractSwitch * self) const;
	};
}

#endif
