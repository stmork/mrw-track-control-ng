//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_REGION_H
#define MRW_MODEL_REGION_H

#include <vector>

#include <QDomElement>

#include <util/method.h>
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
		Section * section(const size_t index) const;

		/**
		 * This method returns the amount of included Section elements.
		 *
		 * @return The Section count.
		 */
		size_t sectionCount() const noexcept;

		/**
		 * This method returns the name of this Region.
		 *
		 * @return  The name of this Region.
		 */
		const QString & name() const noexcept;

		/**
		 * This method returns the projected counting direction used on the
		 * track control screen. The projected counting direction facilitate
		 * the usage of the operating user.
		 *
		 * @return The projection direction of the counting direction for the
		 * GUI.
		 */
		bool direction() const noexcept;

		/**
		 * This method sets the the projected counting direction used on the
		 * track control screen. The projected counting direction facilitate
		 * the usage of the operating user.
		 *
		 * @param settings The QSettings where to get the Region view direction.
		 * @param dir The projection direction of the counting direction for
		 * the GUI.
		 */
		void parse(QSettings & settings, const bool dir) noexcept;

		/**
		 * This method returns a key to determine the logical coordinates from
		 * the description file.
		 *
		 * @return The key for looking up the logical coordinates
		 */
		QString key() const noexcept;

		/**
		 * This template class returns all AssemblyPart elements of the given
		 * type T. The found elements are stored into the given std::vector.
		 *
		 * @param result The result vector collecting the AssembyPart elements
		 * of type T.
		 * @param guard A labmda to fine select if the type T should added to
		 * the result vector.
		 */
		template <class T> constexpr void parts(
			std::vector<T *>          &         result,
			std::function<bool(const T * part)> guard = &mrw::util::Method::always<T>) noexcept
		{
			for (Section * sub : sections)
			{
				sub->parts<T>(result, guard);
			}
		}

		/**
		 * This method writes the preferred GUI view direction into the given
		 * QSettings using the key() method for the value key.
		 *
		 * @param settings The QSettings to write the configuration to.
		 */
		void write(QSettings & settings) const noexcept;

		QString toString() const noexcept override;

	private:
		void add(Section * section) noexcept;
		void link() noexcept;
	};
}

#endif
