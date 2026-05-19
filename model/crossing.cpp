//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <model/modelrailway.h>
#include <model/crossing.h>

using namespace mrw::can;
using namespace mrw::model;

Crossing::Crossing(
	ModelRailway     *    model_railway,
	Controller      *     controller,
	const QDomElement  &  element) :
	Device(model_railway, element),
	crx_controller(controller),
	crx_name(ModelRailway::string(element, "name"))
{
	model_railway->add(this);
}

bool Crossing::valid() const
{
	return crx_sections.size() > 0;
}

bool Crossing::isUsed() const
{
	return std::any_of(
			crx_sections.begin(), crx_sections.end(),
			[] (const Section * section)
	{
		return section->state() != SectionState::FREE;
	});
}

const QString & Crossing::name() const noexcept
{
	return crx_name;
}

bool Crossing::isUnlockable() const noexcept
{
	return true;
}

Controller * Crossing::controller() const noexcept
{
	return crx_controller;
}

MrwMessage Crossing::configMsg(const unsigned int pin) const
{
	MrwMessage msg = command(CFGCRX);

	msg.append(pin);

	return msg;
}

void Crossing::add(Section * section)
{
	crx_sections.emplace_back(section);
}
