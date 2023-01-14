//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_PROFILELIGHT_H
#define MRW_MODEL_PROFILELIGHT_H

#include <model/light.h>

namespace mrw::model
{
	/**
	 * This class represents a LED or bulb light source which can light up
	 * using different light up profiles. Every light can have a configured
	 * lighting threshold. The lighting starts when the sensor values fall
	 * below the threshold. The light turns off when the sensor values go
	 * beyound the threshold + 4 (hysteresis).
	 *
	 * This light is connected to a LightModule instance.
	 *
	 * @see https://github.com/stmork/mrw/wiki/LichtProfile
	 */
	class ProfileLight : public Light
	{
		const unsigned light_profile;

	public:
		explicit ProfileLight(
			ModelRailway     *    model_railway,
			Controller      *     controller,
			const QDomElement  &  element);

		uint8_t profile() const;

		virtual mrw::can::MrwMessage configMsg(const unsigned pin) const override;
	};
}

#endif
