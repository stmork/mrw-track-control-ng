//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/device.h"

using namespace mrw::model;

Device::Device(const QDomElement  &  element) :
	unit_no(ModelRailway::value(element, "unit_no"))
{
}
