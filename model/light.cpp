//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/light.h"

using namespace mrw::model;

Light::Light(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	Device(model_railway, element),
	light_name(ModelRailway::string(element, "name")),
	threshold(ModelRailway::value(element, "schwellwert"))
{
}
