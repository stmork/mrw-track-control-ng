//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <modelrailway.h>

#include "abstractswitch.h"

using namespace mrw::model;

AbstractSwitch::AbstractSwitch(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element) :
	RailPart(model_railway, model_section, element),
	SwitchModuleReference(
		model_railway, element, ModelRailway::boolean(element, "neu"))
{
}

bool AbstractSwitch::isFlankProtection(AbstractSwitch * other) const
{
	return flank_switches.find(other) != flank_switches.end();
}
