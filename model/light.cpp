//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <model/modelrailway.h>
#include <model/light.h>

using namespace mrw::can;
using namespace mrw::model;

Light::Light(
	ModelRailway     *    model_railway,
	Controller      *     controller,
	const QDomElement  &  element) :
	Device(model_railway, element),
	light_controller(controller),
	light_name(ModelRailway::string(element, "name")),
	light_threshold(ModelRailway::value(element, "schwellwert"))
{
	model_railway->add(this);
}

uint8_t Light::threshold() const
{
	return light_threshold;
}

const QString & Light::name() const noexcept
{
	return light_name;
}

bool Light::isUnlockable() const noexcept
{
	return true;
}

Controller * Light::controller() const noexcept
{
	return light_controller;
}

MrwMessage Light::configMsg(const unsigned pin) const
{
	MrwMessage msg = command(CFGLGT);

	msg.append(pin);
	msg.append(threshold());

	return msg;
}
