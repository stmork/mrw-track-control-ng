//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
	threshold(ModelRailway::value(element, "schwellwert"))
{
	model_railway->add(this);
}

const QString & Light::name() const
{
	return light_name;
}

bool Light::isUnlockable() const
{
	return true;
}

Controller * Light::controller() const
{
	return light_controller;
}

MrwMessage Light::configMsg() const
{
	MrwMessage message = command(CFGLGT);

	return message;
}
