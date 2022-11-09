//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODULE_LIGHTMODEL_H
#define MRW_MODULE_LIGHTMODEL_H

#include <vector>

#include <QDomElement>

#include <model/module.h>

namespace mrw::model
{
	class ProfileLight;

	class LightModule : public Module
	{
		std::vector<ProfileLight *> lights;

		static const size_t MAX_LIGHTS = 8;

	public:
		explicit LightModule(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~LightModule();

		inline size_t ports() const override
		{
			return 1;
		}

		bool valid() const override;

	private:
		void link() override;
	};
}

#endif
