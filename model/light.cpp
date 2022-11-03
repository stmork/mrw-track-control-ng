//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/light.h"

using namespace mrw::model;

Light::Light(const QDomElement & element) :
	name(ModelRailway::string(element, "name")),
	unit_no(ModelRailway::value(element, "unit_no")),
	threshold(ModelRailway::value(element, "schwellwert"))
{
}
