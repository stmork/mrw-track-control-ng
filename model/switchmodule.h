//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODULE_SWITCHMODULE_H
#define MRW_MODULE_SWITCHMODULE_H

#include <vector>

#include <QDomElement>

#include <model/module.h>

namespace mrw::model
{
	class SwitchModule : public Module
	{
		std::vector<RailPart *> rail_parts;

	public:
		explicit SwitchModule(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
