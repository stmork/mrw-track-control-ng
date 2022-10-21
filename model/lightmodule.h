//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODULE_LIGHTMODEL_H
#define MRW_MODULE_LIGHTMODEL_H

#include <QDomElement>

#include "model/module.h"

namespace mrw::model
{
	class LightModule : public Module
	{
	public:
		explicit LightModule(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
