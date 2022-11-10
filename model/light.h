//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_LIGHT_H
#define MRW_MODEL_LIGHT_H

#include <QDomElement>

#include <model/module.h>
#include <model/device.h>

namespace mrw::model
{
	/**
	 * This class represents a LED or bulb light source which can only simply
	 * turn on or off the light. Every light can have a configured lighting
	 * threshold. The lighting starts when the sensor values fall below the
	 * threshold. The light turns off when the sensor values go beyound the
	 * threshold + 4 (hysteresis).
	 *
	 * This light is connected to a MultiplexConnection instance.
	 *
	 * @see https://github.com/stmork/mrw/wiki/LichtProfile
	 */
	class Light : public Device
	{
		Controller  *  light_controller;

	protected:
		const QString  light_name;  ///< The light name.
		const unsigned threshold;   ///< The lighting sensor threshold.

	public:
		explicit Light(
			ModelRailway     *    model_railway,
			Controller      *     controller,
			const QDomElement  &  element);
		virtual ~Light() = default;

		inline const QString & name() const override
		{
			return light_name;
		}

		inline Controller * controller() const override
		{
			return light_controller;
		}
	};
}

#endif
