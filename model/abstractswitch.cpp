//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <can/commands.h>
#include <model/modelrailway.h>

#include "abstractswitch.h"

using namespace mrw::can;
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

mrw::can::Command AbstractSwitch::commandState() const
{
	switch (switchState())
	{
	case SwitchState::SWITCH_STATE_LEFT:
		return SETLFT;

	case SwitchState::SWITCH_STATE_RIGHT:
		return SETRGT;
	}

	throw std::invalid_argument("Command state not allowed!");
}
