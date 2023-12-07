//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QtGlobal>

#include "model/modelrailway.h"
#include "model/switchmodule.h"
#include "model/switchmodulereference.h"

using namespace mrw::can;
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
	model_railway->add(this);
}

size_t SwitchModuleReference::inductors() const
{
	return inductor_count;
}

bool SwitchModuleReference::hasCutOff() const
{
	return has_cut_off;
}

SwitchModule * SwitchModuleReference::module() const
{
	return switch_module;
}

Controller * SwitchModuleReference::controller() const
{
	return switch_controller;
}

MrwMessage SwitchModuleReference::configSwitchMsg(
	const Command  cmd,
	const unsigned pin) const
{
	MrwMessage msg = command(cmd);

	for (unsigned p = 0; p < inductors(); p++)
	{
		msg.append(p + pin);
	}

	if (hasCutOff())
	{
		for (unsigned p = 0; p < inductors(); p++)
		{
			msg.append(p + pin + Module::MAX_PINS_PER_PORT);
		}
	}

	return msg;
}
