//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include "model/modelrailway.h"
#include "model/section.h"
#include "model/sectionmodule.h"
#include "model/rail.h"
#include "model/regularswitch.h"
#include "model/doublecrossswitch.h"
#include "model/lightsignal.h"
#include "model/formsignal.h"

using namespace mrw::util;
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
const std::regex  Section::path_regex(R"(^\/\/@controller\.(\d+)\/@module\.(\d+))");

const ConstantEnumerator<SectionState>  Section::state_map
{
	CONSTANT(FREE),
	CONSTANT(SHUNTING),
	CONSTANT(TOUR),
	CONSTANT(OCCUPIED),
	CONSTANT(PASSED)
};

Section::Section(
	ModelRailway     *    model_railway,
	Region        *       region,
	const QDomElement  &  element) :
	Device(model_railway, element),
	section_name(ModelRailway::string(element, "name")),
	model(model_railway),
	section_region(region)
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
				AssemblyPart * rail_part = nullptr;

				if (type == "Gleis")
				{
					rail_part = new Rail(model, this, child);
				}
				else if (type == "Weiche")
				{
					rail_part = new RegularSwitch(model, this,  child);
				}
				else if (type == "DKW")
				{
					rail_part = new DoubleCrossSwitch(model, this, child);
				}
				else if (type == "Gleissperrsignal")
				{
					rail_part = new LightSignal(model, this, child, Signal::SHUNT_SIGNAL, 2);
				}
				else if (type == "Vorsignal")
				{
					rail_part = new LightSignal(model, this, child, Signal::DISTANT_SIGNAL, 4);
				}
				else if (type == "Blocksignal")
				{
					rail_part = new LightSignal(model, this, child, Signal::MAIN_SIGNAL, 2);
				}
				else if (type == "Einfahrsignal")
				{
					rail_part = new LightSignal(model, this, child, Signal::MAIN_SIGNAL, 3);
				}
				else if (type == "Ausfahrsignal")
				{
					rail_part = new LightSignal(model, this, child, Signal::MAIN_SHUNT_SIGNAL, 5);
				}
				else if (type == "Formvorsignal")
				{
					rail_part = new FormSignal(model, this, child, Signal::DISTANT_SIGNAL);
				}
				else if (type == "Formhauptsignal")
				{
					rail_part = new FormSignal(model, this, child, Signal::MAIN_SIGNAL);
				}
				else
				{
					model->warning("Unknown rail type: " + type);
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
	for (AssemblyPart * rail_part : assembly_parts)
	{
		delete rail_part;
	}
	assembly_parts.clear();
}

bool Section::valid() const
{
	return (section_controller != nullptr) && (section_module != nullptr);
}

bool Section::unlockable() const
{
	std::vector<RailPart *> reserved_parts;

	parts<RailPart>(reserved_parts, [](const RailPart * part)
	{
		return part->reserved();
	});

	return (reserved_parts.size() == 0) && (section_state == FREE);
}

SectionState Section::state() const
{
	return occupied ? OCCUPIED : section_state;
}

void Section::add(AssemblyPart * part)
{
	assembly_parts.push_back(part);
}

void Section::link()
{
	for (AssemblyPart * part : assembly_parts)
	{
		if (part != nullptr)
		{
			part->link();
		}
	}
	for (AssemblyPart * part : assembly_parts)
	{
		AbstractSwitch * abstract_switch = dynamic_cast<AbstractSwitch *>(part);

		if (abstract_switch != nullptr)
		{
			abstract_switch->findFlankSwitches();
		}
	}
}

void Section::findSignalPair()
{
	std::vector<Signal *> sction_signals;

	parts<Signal>(sction_signals);
	for (Signal * signal : sction_signals)
	{
		signal->findPair(sction_signals);
	}
}

QString Section::toString() const
{
	return QString("    Sec %1         : [%2] %3 %4 %5").
		arg(valid() ? 'V' : '-').
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name(), -10).
		arg(Device::get(lock()), -10).
		arg(state_map.get(section_state));
}

void Section::free()
{
	for (AssemblyPart * part : assembly_parts)
	{
		RailPart * rail = dynamic_cast<RailPart *>(part);

		if (rail != nullptr)
		{
			rail->reserve(false);
		}
	}
	setState(FREE);
}

SectionModule * Section::resolve(const std::string & path)
{
	std::smatch matcher;

	if (std::regex_match(path, matcher, path_regex))
	{
		Q_ASSERT(matcher.size() >= 3);

		const unsigned controller_idx = std::stoul(matcher[1]);
		const unsigned module_idx     = std::stoul(matcher[2]);

		section_controller = model->controller(controller_idx);
		return dynamic_cast<SectionModule *>(model->module(controller_idx, module_idx));
	}
	return nullptr;
}
