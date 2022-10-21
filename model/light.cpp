//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/mrwmodel.h"
#include "model/light.h"

using namespace mrw::model;

Light::Light(const QDomElement & element) :
	name(ModelRailway::string(element, "name")),
	type(ModelRailway::value(element, "type")),
	threshold(ModelRailway::value(element, "schwellwert"))
{
}
