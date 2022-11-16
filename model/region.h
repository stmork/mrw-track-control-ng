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
#include <model/section.h>

namespace mrw::model
{
	class ModelRailway;

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

		const QString           region_name;
		const bool              is_station;
		ModelRailway      *     model = nullptr;
		std::vector<Section *>  sections;
		bool                    direction_view;


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

		inline const QString & name() const
		{
			return region_name;
		}

		inline bool direction() const
		{
			return direction_view;
		}

		inline void setDirection(const bool dir)
		{
			direction_view = dir;
		}

		QString toString() const override;

		QString key() const;

		/**
		 * This template class returns all AssemblyPart elements of the given
		 * type T. The found elements are stored into the given std::vector.
		 *
		 * @param result The result vector collecting the AssembyPart elements
		 * of type T.
		 */
		template <class T> void parts(std::vector<T *> & result)
		{
			for (Section * sub : sections)
			{
				sub->parts<T>(result);
			}
		}

	private:
		void add(Section * section);
		void link();
	};
}

#endif
