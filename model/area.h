//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_AREA_H
#define MRW_MODEL_AREA_H

#include <vector>

#include <QDomElement>

#include <util/stringutil.h>

namespace mrw::model
{
	class ModelRailway;
	class Section;

	class Area : public mrw::util::String
	{
		const QString           name;
		const bool              is_station;
		ModelRailway      *     model = nullptr;
		std::vector<Section *>  sections;

	public:
		explicit Area(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const bool            station);
		virtual ~Area();

		void add(Section * section);
		void link();

		inline Section * section(const size_t index) const
		{
			return sections.at(index);
		}


		inline size_t sectionCount() const
		{
			return sections.size();
		}

		QString toString() const override;
	};
}

#endif
