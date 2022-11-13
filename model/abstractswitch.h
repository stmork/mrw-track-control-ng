//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_ABSTRACTSWITCH_H
#define MRW_MODEL_ABSTRACTSWITCH_H

#include <unordered_set>

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
	protected:
		/**
		 * The partner switches which may by flank protection switches.
		 */
		std::unordered_set<AbstractSwitch *> flank_switches;

	public:
		explicit AbstractSwitch(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		inline const QString & name() const override
		{
			return part_name;
		}

		bool isFlankProtection(AbstractSwitch * other) const;

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
