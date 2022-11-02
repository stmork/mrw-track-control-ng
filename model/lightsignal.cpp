//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/mrwmodel.h"
#include "model/lightsignal.h"

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
//@controller.9/@anschluesse.1
//@controller.11/@anschluesse.0
@endverbatim
 */
const std::regex  LightSignal::path_regex(R"(^\/\/@controller\.(\d+)\/@anschluesse\.(\d+))");

LightSignal::LightSignal(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const bool            is_main,
	const unsigned        light_count) :
	Signal(model_railway, element, is_main),
	lights(light_count)
{
	const std::string & path = reference.attribute("anschluss").toStdString();
	std::smatch         matcher;

	if (std::regex_match(path, matcher, path_regex))
	{
		Q_ASSERT(matcher.size() >= 3);

		const unsigned controller_idx = std::stoul(matcher[1]);
		const unsigned conn_idx       = std::stoul(matcher[2]);

		mux_connection = model->connection(controller_idx, conn_idx);
	}
	else
	{
		model->warning("Signal " + name + " has no connection module!");
	}
}

void mrw::model::LightSignal::link()
{
}
