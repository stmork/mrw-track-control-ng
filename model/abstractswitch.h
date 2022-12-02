//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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

	protected:
		/**
		 * The partner switches which may by flank protection switches.
		 */
		std::unordered_set<AbstractSwitch *> flank_switches;

	public:
		explicit AbstractSwitch(
			ModelRailway     *    model_railway,
			Section       *       model_section,
			const QDomElement  &  element);

		inline const QString & name() const override
		{
			return partName();
		}

		/**
		 * This method checks whether a connected AbstractSwitch is a flank
		 * protection switch.
		 *
		 * @param other The AbstractSwitch to check.
		 * @return True if the connected AbstractSwitch is a flank protection
		 * switch.
		 */
		bool isFlankProtection(AbstractSwitch * other) const;

		/**
		 * This method converts the internal switch state into the right
		 * token for a MrwMessage;
		 *
		 * @return The turn command for a MrwMessage.
		 */
		virtual mrw::can::SwitchState commandState() const = 0;

		/**
		 * This method sets the internal switch state according to the given
		 * connected neighbours. This is useful while computing the track route.
		 *
		 * @note Both pointers need to be non nullptr.
		 *
		 * @param left One neighbour.
		 * @param right The other neighbour.
		 * @exception std::invalid_argument one of the RailPart pointer is not
		 * a neighbour.
		 */
		virtual void setState(const RailPart * left, const RailPart * right) = 0;

	protected:
		/**
		 * This method finds partner switches which may occur as flank
		 * protection switches. For a RegularSwitch the algorithm is quiet
		 * simple and static whereas the flank protection switch for
		 * DoubleCrossSwitch depends on their state inside an active track.
		 * Nevertheless this method computes all candidates and store them
		 * in dthe flank_switches vector.
		 */
		virtual void findFlankSwitches() = 0;
	};
}

#endif
