//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/device.h"

using namespace mrw::model;

Device::Device(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	unit_no(ModelRailway::value(element, "unit_no"))
{
	if (unit_no == 0)
	{
		model_railway->error(element.nodeName() + " has no unit number!");
	}
}
