//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SECTIONMODULE_H
#define MRW_MODEL_SECTIONMODULE_H

#include <vector>
#include <regex>

#include <QDomElement>

#include <model/module.h>

namespace mrw::model
{
	class Section;

	/**
	 * This class represents a section module which is connected to a CAN
	 * controller using the 16-pin connector.
	 *
	 * @image html section-module.jpg width=40%
	 *
	 * A section module can control up to four sections. A control line
	 * can supply power for driving and signal the occupation of a section.
	 */
	class SectionModule : public Module
	{
		static const std::regex path_regex;

		std::vector<Section *> sections;

		static const size_t MAX_SECTIONS = 4;

	public:
		SectionModule(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		inline size_t ports() const override
		{
			return 1;
		}

		bool valid() const override;

	private:
		void link() override;
		void configure(
			std::vector<mrw::can::MrwMessage> & messages,
			const unsigned                      offset) const override;
	};
}

#endif
