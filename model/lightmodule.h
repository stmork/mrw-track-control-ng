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

	public:
		explicit LightModule(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~LightModule();

		bool valid() const override;

	private:
		void link() override;
	};
}

#endif
