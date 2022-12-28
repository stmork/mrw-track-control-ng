//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QDebug>

#include "model/modelrailway.h"
#include "model/assemblypart.h"

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
//@gruppe.0/@abschnitt.19/@bauelement.1
//@gruppe.2/@abschnitt.10/@bauelement.0
@endverbatim
 */
const std::regex  AssemblyPart::path_regex(R"(^\/\/@gruppe\.(\d+)\/@abschnitt\.(\d+)\/@bauelement\.(\d+))");

AssemblyPart::AssemblyPart(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element) :
	part_name(ModelRailway::string(element, "name")),
	part_model(model_railway),
	part_section(model_section),
	reference(element)
{
}

AssemblyPart * AssemblyPart::resolve(
	const ModelRailway  *  model,
	const QString     &    reference)
{
	std::smatch         matcher;
	const std::string & path = reference.toStdString();

	if (std::regex_match(path, matcher, path_regex))
	{
		Q_ASSERT(matcher.size() >= 4);

		const unsigned region_idx  = std::stoul(matcher[1]);
		const unsigned section_idx = std::stoul(matcher[2]);
		const unsigned part_idx    = std::stoul(matcher[3]);

		return model->assemblyPart(region_idx, section_idx, part_idx);
	}
	return nullptr;
}
