//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/mrwmodel.h>
#include "model/profilelight.h"

using namespace mrw::model;

ProfileLight::ProfileLight(const QDomElement & element) :
	Light(element),
	profile(ModelRailway::value(element, "typ"))
{
}
