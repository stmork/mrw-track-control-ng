//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/modelrailway.h>
#include "model/profilelight.h"

using namespace mrw::model;

ProfileLight::ProfileLight(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	Light(model_railway, element),
	profile(ModelRailway::value(element, "typ"))
{
}
