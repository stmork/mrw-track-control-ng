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
		bool                    direction_view = true;


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

		/**
		 * This method returns the name of this Region.
		 *
		 * @return  The name of this Region.
		 */
		inline const QString & name() const
		{
			return region_name;
		}

		/**
		 * This method returns the projected counting direction used on the
		 * track control screen. The projected counting direction facilitate
		 * the usage of the operating user.
		 *
		 * @return The projection direction of the counting direction for the
		 * GUI.
		 */
		inline bool direction() const
		{
			return direction_view;
		}

		/**
		 * This method sets the the projected counting direction used on the
		 * track control screen. The projected counting direction facilitate
		 * the usage of the operating user.
		 *
		 * @param settings The QSettings where to get the Region view direction.
		 * @param dir The projection direction of the counting direction for
		 * the GUI.
		 */
		void parse(QSettings & settings, const bool dir);

		/**
		 * This method returns a key to determine the logical coordinates from
		 * the description file.
		 *
		 * @return The key for looking up the logical coordinates
		 */
		QString key() const;

		/**
		 * This template class returns all AssemblyPart elements of the given
		 * type T. The found elements are stored into the given std::vector.
		 *
		 * @param result The result vector collecting the AssembyPart elements
		 * of type T.
		 * @param guard A labmda to fine select if the type T should added to
		 * the result vector.
		 */
		template <class T> void parts(std::vector<T *> & result,
			std::function<bool(const T * part)> guard = [](const T * part)
		{
			(void)part;

			return true;
		})
		{
			for (Section * sub : sections)
			{
				sub->parts<T>(result, guard);
			}
		}

		void write(QSettings & settings) const;
		QString toString() const override;

	private:
		void add(Section * section);
		void link();
	};
}

#endif
