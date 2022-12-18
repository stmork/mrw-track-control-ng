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
	Section       *       model_section,
	const QDomElement  &  element,
	const SignalType      type,
	const unsigned        light_count) :
	Signal(
		model_railway, model_section, element, type),
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

		signal_controller = part_model->controller(controller_idx);
		mux_connection    = part_model->connection(controller_idx, conn_idx);
	}
	else
	{
		part_model->warning("Signal " + part_name + " has no connection module!");
	}
	model_railway->add(this);
}

bool LightSignal::valid() const
{
	return (signal_controller != nullptr) && (mux_connection != nullptr);
}

Controller * LightSignal::controller() const
{
	return signal_controller;
}

MultiplexConnection * LightSignal::connection() const
{
	return mux_connection;
}

const QString & LightSignal::name() const
{
	return part_name;
}

QString LightSignal::toString() const
{
	return QString("      L %1  %2   : [%3] %4 %5").
		arg(valid()  ? "V" : "-").
		arg(symbol()).
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name(), -10).
		arg(Device::get(lock()), -10);
}

size_t LightSignal::usedPins() const
{
	return lights;
}

Device * LightSignal::device()
{
	return this;
}
