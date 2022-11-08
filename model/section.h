//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SECTION_H
#define MRW_MODEL_SECTION_H

#include <vector>
#include <regex>

#include <QDomElement>

#include <util/stringutil.h>
#include <model/module.h>
#include <model/device.h>

namespace mrw::model
{
	class ModelRailway;
	class SectionModule;
	class AssemblyPart;

	/**
	 * A Section manages exactly one occupation state for a RailPart
	 * collection. It may also manage several Signal instances. The Section
	 * is controlled by a SectionModule.
	 */
	class Section : public Device, public mrw::util::String
	{
		friend class Region;

		static const std::regex      path_regex;

		const QString                section_name;
		ModelRailway        *        model              = nullptr;
		Controller         *         section_controller = nullptr;
		SectionModule        *       section_module     = nullptr;
		std::vector<AssemblyPart *>  rail_parts;

	public:
		explicit Section(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~Section();

		inline const QString & name() const override
		{
			return section_name;
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
			return rail_parts.at(index);
		}

		/**
		 * This method returns the amount of RailPart elements contained in
		 * this Section.
		 *
		 * @return The contained RailPart count.
		 */
		inline size_t railPartCount() const
		{
			return rail_parts.size();
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

	private:
		void            add(AssemblyPart * rail_part);
		void            link();
		SectionModule * resolve(const std::string & path);
	};
}

#endif // MRW_MODEL_SECTION_H
