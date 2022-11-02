//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SECTIONMODULE_H
#define MRW_MODEL_SECTIONMODULE_H

#include <vector>
#include <regex>

#include <QDomElement>

#include <model/module.h>

namespace mrw::model
{
	class Section;

	class SectionModule : public Module
	{
		static const std::regex path_regex;

		std::vector<Section *> sections;

	public:
		SectionModule(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		void link() override;
	};
}

#endif
