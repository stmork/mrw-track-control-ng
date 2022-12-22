//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/light.h"

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

Controller * Light::controller() const
{
	return light_controller;
}

bool Light::notUnlockable() const
{
	return true;
}
