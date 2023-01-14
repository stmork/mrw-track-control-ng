//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <model/modelrailway.h>
#include "model/profilelight.h"

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
