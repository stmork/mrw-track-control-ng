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

	/**
	 * The control region is a structural division into overviewable parts
	 * of the model railway. It has no impact onto the electronical control
	 * domain. The region can be distinguished into station region and
	 * outside rail region. You can use station region for shunting inside
	 * whereas on the free rail region no shunting is allowed.
	 */
	class Region : public mrw::util::String
	{
		const QString           name;
		const bool              is_station;
		ModelRailway      *     model = nullptr;
		std::vector<Section *>  sections;

	public:
		explicit Region(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const bool            station);
		virtual ~Region();

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
