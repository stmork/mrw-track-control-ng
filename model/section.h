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

	class Section : public Device, public mrw::util::String
	{
		static const std::regex      path_regex;

		const QString                section_name;
		ModelRailway        *        model  = nullptr;
		SectionModule        *       section_module = nullptr;
		std::vector<AssemblyPart *>  rail_parts;

	public:
		explicit Section(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~Section();

		void add(AssemblyPart * rail_part);
		void link();

		inline const QString & name() const override
		{
			return section_name;
		}

		QString toString() const override;

		inline AssemblyPart * assemblyPart(const int idx) const
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
