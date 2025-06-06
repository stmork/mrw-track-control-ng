//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_LIGHTMODEL_H
#define MRW_MODEL_LIGHTMODEL_H

#include <QDomElement>

#include <model/module.h>
#include <util/cleanvector.h>

namespace mrw::model
{
	class ProfileLight;
	class Controller;

	/**
	 * This module connects light sources such as LEDs or light bulbs. The
	 * lighting up may follow different profiles using PWM of the controlling
	 * CAN controller. The lighting starts when the sensor values fall below
	 * the threshold. The light turns off when the sensor values go beyound
	 * the threshold + 4 (hysteresis).
	 *
	 * @see https://github.com/stmork/mrw/wiki/LichtProfile
	 */
	class LightModule : public Module
	{
		mrw::util::CleanVector<ProfileLight> profile_lights;

		static constexpr size_t MAX_LIGHTS = 8;

	public:
		explicit LightModule(
			ModelRailway     *    model_railway,
			Controller      *     controller,
			const QDomElement  &  element);
		virtual ~LightModule() = default;

		inline size_t ports() const override
		{
			return 1;
		}

		bool valid() const override;

		const std::vector<ProfileLight *> & lights() const;

	private:
		void link() override;
		void configure(
			std::vector<mrw::can::MrwMessage> & messages,
			const unsigned                      offset) const override;
	};
}

#endif
