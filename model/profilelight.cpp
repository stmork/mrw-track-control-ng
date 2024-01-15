//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <model/modelrailway.h>
#include "model/profilelight.h"

using namespace mrw::can;
using namespace mrw::model;

ProfileLight::ProfileLight(
	ModelRailway     *    model_railway,
	Controller      *     controller,
	const QDomElement  &  element) :
	Light(model_railway, controller, element),
	light_profile(ModelRailway::value(element, "typ"))
{
}

uint8_t ProfileLight::profile() const
{
	return light_profile;
}

MrwMessage ProfileLight::configMsg(const unsigned pin) const
{
	MrwMessage msg = command(CFGLGT);

	msg.append(pin);
	msg.append(threshold());
	msg.append(profile());

	return msg;
}
