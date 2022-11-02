//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include "model/mrwmodel.h"
#include "model/section.h"
#include "model/sectionmodule.h"
#include "model/rail.h"
#include "model/railswitch.h"
#include "model/raildoubleswitch.h"
#include "model/lightsignal.h"
#include "model/formsignal.h"

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
const std::regex  Section::path_regex(R"(^\/\/@controller\.(\d+)\/@module\.(\d+))");

Section::Section(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	name(ModelRailway::string(element, "name")),
	unit_no(ModelRailway::value(element, "unit_no")),
	model(model_railway)
{
	const QDomNodeList & child_nodes = element.childNodes();

	for (int n = 0; n < child_nodes.count(); ++n)
	{
		const QDomNode & node = child_nodes.at(n);

		if (node.isElement())
		{
			const QDomElement & child = node.toElement();
			const QString       type  = ModelRailway::type(child);

			if (node.nodeName() == "bauelement")
			{
				RailPart * rail_part = nullptr;

				if (type == "Gleis")
				{
					rail_part = new Rail(model, child);
				}
				else if (type == "Weiche")
				{
					rail_part = new RailSwitch(model, child);
				}
				else if (type == "DKW")
				{
					rail_part = new RailDoubleSwitch(model, child);
				}
				else if (type == "Gleissperrsignal")
				{
					rail_part = new LightSignal(model, child, false, 2);
				}
				else if (type == "Vorsignal")
				{
					rail_part = new LightSignal(model, child, false, 4);
				}
				else if (type == "Blocksignal")
				{
					rail_part = new LightSignal(model, child, true, 2);
				}
				else if (type == "Einfahrsignal")
				{
					rail_part = new LightSignal(model, child, true, 3);
				}
				else if (type == "Ausfahrsignal")
				{
					rail_part = new LightSignal(model, child, true, 5);
				}
				else if (type == "Formvorsignal")
				{
					rail_part = new FormSignal(model, child, false);
				}
				else if (type == "Formhauptsignal")
				{
					rail_part = new FormSignal(model, child, true);
				}
				else
				{
					qWarning() << "Unknown rail type" << type;
				}
				add(rail_part);
			}
			else
			{
				model->error("Unknown rail part: " + node.nodeName());
			}
		}
	}

	section_module = resolve(ModelRailway::string(element, "modul").toStdString());
}

Section::~Section()
{
	for (RailPart * rail_part : rail_parts)
	{
		delete rail_part;
	}
	rail_parts.clear();
}

void Section::add(RailPart * rait_part)
{
	rail_parts.push_back(rait_part);
}

void Section::link()
{
	for (RailPart * rail_part : rail_parts)
	{
		if (rail_part != nullptr)
		{
			rail_part->link();
		}
	}
}

SectionModule * Section::resolve(const std::string & path) const
{
	std::smatch matcher;

	if (std::regex_match(path, matcher, path_regex))
	{
		Q_ASSERT(matcher.size() >= 3);

		const unsigned constroller_idx = std::stoul(matcher[1]);
		const unsigned module_idx      = std::stoul(matcher[2]);

		return static_cast<SectionModule *>(model->module(constroller_idx, module_idx));
	}
	return nullptr;
}
