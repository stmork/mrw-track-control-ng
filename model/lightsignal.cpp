//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/lightsignal.h"

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
//@controller.9/@anschluesse.1
//@controller.11/@anschluesse.0
@endverbatim
 */
const std::regex  LightSignal::path_regex(R"(^\/\/@controller\.(\d+)\/@anschluesse\.(\d+))");

LightSignal::LightSignal(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const SignalType      type,
	const unsigned        light_count) :
	Signal(model_railway, element, type),
	Device(model_railway, element),
	lights(light_count)
{
	const std::string & path = reference.attribute("anschluss").toStdString();
	std::smatch         matcher;

	if (std::regex_match(path, matcher, path_regex))
	{
		Q_ASSERT(matcher.size() >= 3);

		const unsigned controller_idx = std::stoul(matcher[1]);
		const unsigned conn_idx       = std::stoul(matcher[2]);

		signal_controller = model->controller(controller_idx);
		mux_connection    = model->connection(controller_idx, conn_idx);
	}
	else
	{
		model->warning("Signal " + part_name + " has no connection module!");
	}
}

bool LightSignal::valid() const
{
	return (signal_controller != nullptr) && (mux_connection != nullptr);
}

QString LightSignal::toString() const
{
	return QString("      L %1  %2  : %3").
		arg(valid()  ? "V" : "-").arg(symbol()).arg(name());
}

void LightSignal::findPair(const std::vector<LightSignal *> & light_signals)
{
	if (signal_type == MAIN_SIGNAL)
	{
		for (LightSignal * signal : light_signals)
		{
			if ((signal->signal_type == DISTANT_SIGNAL) &&
				(signal_direction == signal->signal_direction))
			{
				main_distant_pair = signal;
			}
		}
	}
}
