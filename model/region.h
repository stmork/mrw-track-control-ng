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
		friend class ModelRailway;

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

		/**
		 * This method returns the nth Section element. It is not ID-based
		 * but index based and is used for linking the Sections after the
		 * model has loaded into memory.
		 *
		 * @param index The zero based index of the Section.
		 * @return The found Section instance.
		 *
		 * @see link()
		 */
		inline Section * section(const size_t index) const
		{
			return sections.at(index);
		}

		/**
		 * This method returns the amount of included Section elements.
		 *
		 * @return The Section count.
		 */
		inline size_t sectionCount() const
		{
			return sections.size();
		}

		QString toString() const override;

	private:
		void add(Section * section);
		void link();
	};
}

#endif
