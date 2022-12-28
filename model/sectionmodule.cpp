//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QStringList>

#include "util/stringutil.h"
#include "model/modelrailway.h"
#include "model/section.h"
#include "model/sectionmodule.h"

using namespace mrw::model;

/**
 * Regular expression prolog:
 * \d matches one digit.
 * \s whitespace character
 * \w matches one digit or case insensitive letter.
 * "." matches one character.
 * \d+ matches a number with at least one digit
 * (\d+) marks a matching group
 *
 * Since we use C++11 raw string literals we do not need to escape the '\' char to "\\".
 *
 * There is a nice regular expression test site where you can build a regular expression and test it agai
 * multiple test patterns: https://regex101.com/
 *
 * The following test patterns may be used:
 * @verbatim
//@gruppe.0/@abschnitt.8
//@gruppe.1/@abschnitt.18
//@gruppe.12/@abschnitt.17
//@gruppe.31/@abschnitt.15
@endverbatim
 */
const std::regex  SectionModule::path_regex(R"(^\/\/@gruppe\.(\d+)\/@abschnitt\.(\d+))");

SectionModule::SectionModule(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) : Module(model_railway, element)
{
}

bool SectionModule::valid() const
{
	return sections.size() <= MAX_SECTIONS;
}

void SectionModule::link()
{
	QStringList references = reference.attribute("abschnitte").split(' ', SKIP_EMPTY_PARTS);

	for (const QString & part_reference : references)
	{
		std::smatch         matcher;
		const std::string & path = part_reference.toStdString();

		if (std::regex_match(path, matcher, path_regex))
		{
			Q_ASSERT(matcher.size() >= 3);

			const unsigned region_idx  = std::stoul(matcher[1]);
			const unsigned section_idx = std::stoul(matcher[2]);

			Section * section = model->section(region_idx, section_idx);
			sections.push_back(section);
		}
	}
}
