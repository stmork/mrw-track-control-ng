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

#include <model/module.h>

namespace mrw::model
{
	class ModelRailway;
	class SectionModule;
	class AssemblyPart;

	class Section
	{
		static const std::regex  path_regex;

		const QString            name;
		const UnitNo             unit_no;
		ModelRailway      *      model  = nullptr;
		SectionModule      *     section_module = nullptr;
		std::vector<AssemblyPart *>  rail_parts;

	public:
		explicit Section(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~Section();

		inline UnitNo id() const
		{
			return unit_no;
		}

		void add(AssemblyPart * rait_part);
		void link();

		inline AssemblyPart * railPart(const int idx) const
		{
			return rail_parts.at(idx);
		}

		inline size_t railPartCount() const
		{
			return rail_parts.size();
		}

		inline SectionModule * module() const
		{
			return section_module;
		}

	protected:
		SectionModule * resolve(const std::string & path) const;
	};
}

#endif // MRW_MODEL_SECTION_H
