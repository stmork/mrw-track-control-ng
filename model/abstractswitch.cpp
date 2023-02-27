//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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

bool AbstractSwitch::isUnlockable() const
{
	return !reserved();
}

MrwMessage AbstractSwitch::configMsg(const unsigned pin) const
{
	return configSwitchMsg(hasCutOff() ? CFGSWN : CFGSWO, pin);
}

const QString & AbstractSwitch::name() const
{
	return partName();
}

RegularSwitch * AbstractSwitch::follow(RailPart * part) const
{
	Rail * rail = dynamic_cast<Rail *>(part);

	if (rail != nullptr)
	{
		// If this is backward
		if (rail->contains(this, false))
		{
			// Take forward.
			part = *rail->advance(false).begin();
		}

		// If this is forward
		if (rail->contains(this, true))
		{
			// Take backward.
			part = *rail->advance(true).begin();
		}
	}
	return dynamic_cast<RegularSwitch *>(part);
}

bool AbstractSwitch::isFlankCandidate(
	const RegularSwitch * candidate,
	const bool            left) const
{
	return candidate != nullptr ? linked(left ? candidate->b : candidate->c, this) : false;
}

bool AbstractSwitch::linked(
	const RailPart    *    candidate,
	const AbstractSwitch * self) const
{
	const Rail * rail = dynamic_cast<const Rail *>(candidate);

	if (candidate == self)
	{
		// OK, directly connected.
		return true;
	}

	if (rail != nullptr)
	{
		// Test if indirectly connected using a simple Rail.
		return rail->contains(self, true) != rail->contains(self, false);
	}
	return false;
}
