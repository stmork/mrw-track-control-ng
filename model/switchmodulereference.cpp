//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QtGlobal>

#include "model/modelrailway.h"
#include "model/switchmodule.h"
#include "model/switchmodulereference.h"

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
//@controller.3/@module.0
//@controller.7/@module.1
@endverbatim
 */
const std::regex  SwitchModuleReference::path_regex(R"(^\/\/@controller\.(\d+)\/@module\.(\d+))");

SwitchModuleReference::SwitchModuleReference(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const bool            cutoff) :
	Device(model_railway, element),
	inductor_count(model_railway->value(element, "spulen", 2)),
	has_cut_off(cutoff)
{
	std::string path = ModelRailway::string(element, "modul").toStdString();
	std::smatch matcher;

	if (std::regex_match(path, matcher, path_regex))
	{
		Q_ASSERT(matcher.size() >= 3);

		const unsigned constroller_idx = std::stoul(matcher[1]);
		const unsigned module_idx      = std::stoul(matcher[2]);

		switch_controller = model_railway->controller(constroller_idx);
		switch_module     = dynamic_cast<SwitchModule *>(
				model_railway->module(constroller_idx, module_idx));
	}
	else
	{
		model_railway->error("Inductive rail part " + element.nodeName() + " has no switch module!");
	}
}
