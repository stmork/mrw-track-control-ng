//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include "model/modelrailway.h"
#include "model/assemblypart.h"

using namespace mrw::model;

/**
 * Regular expression prolog:
 * \d matches one digit.
 * \w matches one digit or case insensitive letter.
 * "." matches one character.
 * \d+ matches a number with at least one digit
 * (\d+) marks a matching group
 *
 *
 * This initializes the regular expressions as follows:
 *
 * First group: (\d+) Collects the tag number in match 1.
 * At least one whitespace.
 * Second group ([A-Z\d_]+) matches at least one of upper case letters, digits and "_" which is the chara
 * set of a cipher.
 * At least one whitespace.
 * The third group has to match the constant keywort "inline:".
 * The fourth group ([\w\/+=]+) matches all alpha numeric letters (lower case and upper case) and additio
 * "/", "+" and "=" which matches all possible characters of a BASE64 encoded string.
 * All further groups may match a beginning "|" with at least one letter. Since "|" is a keyword for divi
 * alternatives it has to be escaped by "\". The regex is: (\|.+)*
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
	const QDomElement  &  element) :
	part_name(ModelRailway::string(element, "name")),
	model(model_railway),
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
