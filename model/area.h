//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_AREA_H
#define MRW_MODEL_AREA_H

#include <vector>

#include <QDomElement>

#include "model/module.h"

namespace mrw::model
{
	class ModelRailway;
	class Section;

	class Area
	{
		const QString           name;
		ModelRailway      *     model = nullptr;
		std::vector<Section *>  sections;

	public:
		explicit Area(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~Area();

		void add(Section * section);
		void link();

		inline Section * section(const UnitNo index) const
		{
			return sections.at(index);
		}
	};
}

#endif
