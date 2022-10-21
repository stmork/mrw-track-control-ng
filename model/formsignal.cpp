//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/mrwmodel.h"
#include "model/formsignal.h"
#include "model/switchmodule.h"

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
//@controller.13/@module.0
//@controller.12/@module.1
@endverbatim
 */
const std::regex  FormSignal::path_regex(R"(^\/\/@controller\.(\d+)\/@module\.(\d+))");

FormSignal::FormSignal(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const bool            is_main) :
	Signal(model_railway, element, is_main),
	inductors(model->value(element, "spulen"))
{
}

void mrw::model::FormSignal::link()
{
	const QString & value = reference.attribute("modul");
	const std::string & path = value.toStdString();
	std::smatch         matcher;

	if (std::regex_match(path, matcher, path_regex))
	{
		Q_ASSERT(matcher.size() >= 3);

		const unsigned controller_idx = std::stoul(matcher[1]);
		const unsigned module_idx     = std::stoul(matcher[2]);

		module = static_cast<SwitchModule *>(model->module(controller_idx, module_idx));
	}
}
