//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QDebug>

#include "model/modelrailway.h"
#include "model/railpart.h"

using namespace mrw::model;

RailPart::RailPart(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element) :
	AssemblyPart(model_railway, model_section, element),
	a_in_dir(ModelRailway::boolean(element, "aInZaehlrichtung"))
{
}

bool RailPart::contains(const RailPart * rail, const bool dir) const noexcept
{
	const std::set<RailInfo> & rails = advance(!dir);

	return std::any_of(rails.begin(), rails.end(), [rail] (const RailInfo & info)
	{
		return info == rail;
	});
}

RailPart * RailPart::resolve(const char * attr) const noexcept
{
	const QString & value = reference.attribute(attr, "");

	return dynamic_cast<RailPart *>(AssemblyPart::resolve(part_model, value));
}

std::set<RailInfo> & RailPart::advance(const bool dir) noexcept
{
	return dir ? rail_forward : rail_backward;
}

const std::set<RailInfo> & RailPart::advance(const bool dir) const noexcept
{
	return dir ? rail_forward : rail_backward;
}

void RailPart::reserve(const bool input) noexcept
{
	is_reserved = input;
}

bool RailPart::reserved() const noexcept
{
	return is_reserved;
}

RailInfo::RailInfo(
	RailPart * rail_part,
	const bool preferred,
	const bool curved) : rail(rail_part)
{
	std::underlying_type_t<PreferCode> flags = 0;

	if (!preferred)
	{
		flags |= std::underlying_type_t<PreferCode>(PreferCode::INFERIOR_FLAG);
	}
	if (curved)
	{
		flags |= std::underlying_type_t<PreferCode>(PreferCode::CURVED_FLAG);
	}
	code = static_cast<PreferCode>(flags);
}

bool RailInfo::operator<(const RailInfo & other) const
{
	return code < other.code;
}
