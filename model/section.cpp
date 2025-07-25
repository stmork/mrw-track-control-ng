//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QDebug>

#include <can/mrwmessage.h>
#include <model/modelrailway.h>
#include <model/section.h>
#include <model/sectionmodule.h>
#include <model/rail.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>
#include <model/lightsignal.h>
#include <model/formsignal.h>

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::model;

using SignalType = Signal::SignalType;

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
	{ SectionState::FREE,     "FREE"},
	{ SectionState::SHUNTING, "SHUNTING"},
	{ SectionState::TOUR,     "TOUR"},
	{ SectionState::OCCUPIED, "OCCUPIED"},
	{ SectionState::PASSED,   "PASSED"}
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

	assembly_parts.reserve(child_nodes.count());
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
					rail_part = new LightSignal(model, this, child, SignalType::SHUNT_SIGNAL, 2);
				}
				else if (type == "Vorsignal")
				{
					rail_part = new LightSignal(model, this, child, SignalType::DISTANT_SIGNAL, 4);
				}
				else if (type == "Blocksignal")
				{
					rail_part = new LightSignal(model, this, child, SignalType::MAIN_SIGNAL, 2);
				}
				else if (type == "Einfahrsignal")
				{
					rail_part = new LightSignal(model, this, child, SignalType::MAIN_SIGNAL, 3);
				}
				else if (type == "Ausfahrsignal")
				{
					rail_part = new LightSignal(model, this, child, SignalType::MAIN_SHUNT_SIGNAL, 5);
				}
				else if (type == "Formgleissperrsignal")
				{
					rail_part = new FormSignal(model, this, child, SignalType::SHUNT_SIGNAL);
				}
				else if (type == "Formvorsignal")
				{
					rail_part = new FormSignal(model, this, child, SignalType::DISTANT_SIGNAL);
				}
				else if (type == "Formhauptsignal")
				{
					rail_part = new FormSignal(model, this, child, SignalType::MAIN_SIGNAL);
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
	model->add(this);

	forward_signals.reserve(3);
	parts<Signal>(forward_signals, [&](const Signal * signal)
	{
		return signal->direction();
	});

	backward_signals.reserve(3);
	parts<Signal>(backward_signals, [&](const Signal * signal)
	{
		return !signal->direction();
	});

	std::sort(forward_signals.begin(),  forward_signals.end(),  Signal::less);
	std::sort(backward_signals.begin(), backward_signals.end(), Signal::less);
}

bool Section::valid() const noexcept
{
	return (section_controller != nullptr) && (section_module != nullptr);
}

const QString & Section::name() const noexcept
{
	return section_name;
}

void Section::setOccupation(const bool input) noexcept
{
	section_occupied = input;
}

bool Section::occupation() const noexcept
{
	return section_occupied;
}

void Section::enable(const bool enable) noexcept
{
	section_enabled = enable;
}

void Section::disable(const bool disable) noexcept
{
	section_enabled = !disable;
}

bool Section::enabled() const noexcept
{
	return section_enabled;
}

bool Section::isUnlockable() const noexcept
{
	std::vector<RailPart *> reserved_parts;

	parts<RailPart>(reserved_parts, [](const RailPart * part)
	{
		return part->reserved();
	});

	return
		(reserved_parts.size() == 0) &&
		(section_state == SectionState::FREE);
}

SectionState Section::state() const noexcept
{
	return section_occupied ? SectionState::OCCUPIED : section_state;
}

void Section::setState(const SectionState input) noexcept
{
	section_state = input;
}

AssemblyPart * Section::assemblyPart(const int index) const
{
	return assembly_parts.at(index);
}

size_t Section::assemblyPartCount() const noexcept
{
	return assembly_parts.size();
}

SectionModule * Section::module() const noexcept
{
	return section_module;
}

Controller * Section::controller() const noexcept
{
	return section_controller;
}

MrwMessage Section::configMsg(const unsigned pin) const
{
	MrwMessage msg = command(CFGRAI);

	msg.append(pin);
	msg.append(pin ^ 7);

	return msg;
}

Region * Section::region() const noexcept
{
	return section_region;
}

void Section::add(AssemblyPart * part) noexcept
{
	assembly_parts.push_back(part);
}

void Section::link() noexcept
{
	for (AssemblyPart * part : assembly_parts)
	{
		if (part != nullptr)
		{
			part->link();
		}
	}
}

QString Section::toString() const
{
	static const QString O = String::format(String::BOLD_ON + String::RED_ON, "O");

	return QString("    Sec %1%2%3       : [%4] %5 %6 %7").
		arg(valid() ?      'V' : '-').
		arg(occupation() ? O : "-").
		arg(enabled() ? "e" : "-").
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name(), -10).
		arg(Device::get(lock()), -10).
		arg(state_map.get(section_state));
}

void Section::free() noexcept
{
	for (AssemblyPart * part : assembly_parts)
	{
		RailPart * rail_part = dynamic_cast<RailPart *>(part);

		if (rail_part != nullptr)
		{
			rail_part->reserve(false);
		}
	}
	setState(SectionState::FREE);
}

bool Section::isFree() const noexcept
{
	return section_state == SectionState::FREE;
}

bool Section::isFlankRelevant() const noexcept
{
	return
		(section_state == SectionState::TOUR) ||
		(section_state == SectionState::PASSED);
}

QString Section::key() const noexcept
{
	return name();
}

bool Section::anyReserved() const noexcept
{
	std::vector<RailPart *> rails;

	parts<RailPart>(rails, [](const RailPart * part)
	{
		return part->reserved();
	});
	return rails.size() > 0;
}

QString Section::get(const SectionState & state) noexcept
{
	return state_map.get(state);
}

const std::vector<Signal *> & Section::getSignals(const bool view) const noexcept
{
	return view ? forward_signals : backward_signals;
}

SectionModule * Section::resolve(const std::string & path) noexcept
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
