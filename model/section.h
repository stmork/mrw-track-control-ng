//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SECTION_H
#define MRW_MODEL_SECTION_H

#include <vector>
#include <regex>
#include <functional>
#include <type_traits>

#include <QDomElement>

#include <util/stringutil.h>
#include <model/module.h>
#include <model/device.h>
#include <model/position.h>

namespace mrw::model
{
	class ModelRailway;
	class Region;
	class SectionModule;
	class AssemblyPart;

	enum SectionState
	{
		FREE,
		SHUNTING,
		TOUR,
		OCCUPIED,
		PASSED
	};

	/**
	 * A Section manages exactly one occupation state for a RailPart
	 * collection. It may also manage several Signal instances. The Section
	 * is controlled by a SectionModule.
	 */
	class Section : public Device, public Position, public mrw::util::String
	{
		friend class Region;

		static const std::regex      path_regex;

		const QString                section_name;
		ModelRailway        *        model              = nullptr;
		Region           *           section_region     = nullptr;
		Controller         *         section_controller = nullptr;
		SectionModule        *       section_module     = nullptr;
		SectionState                 section_state      = FREE;
		bool                         occupied           = false;
		std::vector<AssemblyPart *>  assembly_parts;

	public:
		explicit Section(
			ModelRailway     *    model_railway,
			Region        *       region,
			const QDomElement  &  element);
		virtual ~Section();

		/**
		 * This method validates if all references are configured correctly.
		 *
		 * @return True if every configuration is fine.
		 */
		bool valid() const;

		inline const QString & name() const override
		{
			return section_name;
		}

		inline void setOccupation(const bool input = true)
		{
			occupied = input;
		}

		inline bool occupation() const
		{
			return occupied;
		}

		SectionState state() const;
		void setState(const SectionState input)
		{
			section_state = input;
		}

		QString toString() const override;

		/**
		 * This method returns the nth RailPart. It is not ID-based but index
		 * based and is used for linking the RailPart elements after the
		 * model has loaded into memory.
		 *
		 * @param index The zero based index of connected RailPart.
		 * @return The found RailPart instance.
		 */
		inline AssemblyPart * assemblyPart(const int index) const
		{
			return assembly_parts.at(index);
		}

		/**
		 * This method returns the amount of RailPart elements contained in
		 * this Section.
		 *
		 * @return The contained RailPart count.
		 */
		inline size_t railPartCount() const
		{
			return assembly_parts.size();
		}

		/**
		 * This method returns the controlling SectionModule.
		 *
		 * @return The controlling SectionModule.
		 */
		inline SectionModule * module() const
		{
			return section_module;
		}

		/**
		 * This method returns the CAN Controller to which the controlling
		 * SectionModule is connected to.
		 *
		 * @return The CAN Controller of the controlling SectionModule.
		 */
		inline Controller * controller() const override
		{
			return section_controller;
		}

		/**
		 * This method returns the Region to which this Section is belonging to.
		 *
		 * @return The Region of this Section.
		 */
		inline Region * region() const
		{
			return section_region;
		}

		/**
		 * This template class returns all AssemblyPart elements of the given
		 * type T. The found elements are stored into the given std::vector.
		 *
		 * @param result The result vector collecting the AssembyPart elements
		 * of type T.
		 */
		template <class T> void parts(
			std::vector<T *> & result,
			std::function<bool(const T * part)> guard = [](const T * part)
		{
			(void)part;

			return true;
		})
		{
			for (AssemblyPart * part : assembly_parts)
			{
				T * element = dynamic_cast<T *>(part);

				if ((element != nullptr) && guard(element))
				{
					result.push_back(element);
				}
			}
		}

		virtual QString key() const override
		{
			return name();
		}

	private:
		void            add(AssemblyPart * rail_part);
		void            link();
		void            findSignalPair();
		SectionModule * resolve(const std::string & path);
	};
}

#endif // MRW_MODEL_SECTION_H
