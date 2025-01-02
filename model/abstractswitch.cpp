//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <model/modelrailway.h>
#include <model/regularswitch.h>
#include <model/rail.h>

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

Command AbstractSwitch::commandState() const
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

bool AbstractSwitch::isUnlockable() const noexcept
{
	return !reserved();
}

MrwMessage AbstractSwitch::configMsg(const unsigned pin) const noexcept
{
	return configSwitchMsg(hasCutOff() ? CFGSWN : CFGSWO, pin);
}

bool AbstractSwitch::isFlankProtection(const AbstractSwitch * other) const noexcept
{
	return
		(other != nullptr) &&
		(std::find(flank_switches.begin(), flank_switches.end(), other) != flank_switches.end());
}

const QString & AbstractSwitch::name() const noexcept
{
	return partName();
}

/*************************************************************************
**                                                                      **
**       Flank protection methods                                       **
**                                                                      **
*************************************************************************/

RegularSwitch * AbstractSwitch::follow(
	RailPart * part,
	const bool dir,
	const bool left) const noexcept
{
	const RailPart * last  = this;
	Rail      *      rail  = dynamic_cast<Rail *>(part);
	size_t           count = 0;

	while ((rail != nullptr) && (rail->region() == region()) && (count++ < MAX_FOLLOW_RAIL))
	{
		last = part;
		part = *rail->advance(dir).begin();
		rail = dynamic_cast<Rail *>(part);
	}
	RegularSwitch * candidate = dynamic_cast<RegularSwitch *>(part);

	if ((candidate != nullptr) && (last != (left ? candidate->b : candidate->c)))
	{
		candidate = nullptr;
	}

	return candidate;
}
