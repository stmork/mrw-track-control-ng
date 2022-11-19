//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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

bool RailPart::contains(const RailPart * rail, const bool dir) const
{
	const std::set<RailInfo> & rails = advance(!dir);

	return std::any_of(rails.begin(), rails.end(), [rail] (const RailInfo & info)
	{
		return info == rail;
	});
}

RailPart * RailPart::resolve(const char * attr) const
{
	const QString & value = reference.attribute(attr, "");

	return dynamic_cast<RailPart *>(AssemblyPart::resolve(part_model, value));
}

std::set<RailInfo> & RailPart::advance(const bool dir)
{
	return dir ? rail_forward : rail_backward;
}

const std::set<RailInfo> & RailPart::advance(const bool dir) const
{
	return dir ? rail_forward : rail_backward;
}

RailInfo::RailInfo(
	RailPart * rail_part,
	const bool preferred,
	const bool curved) : rail(rail_part)
{
	unsigned flags = 0;

	if (!preferred)
	{
		flags |= PREFERRED_FLAG;
	}
	if (curved)
	{
		flags |= CURVED_FLAG;
	}
	code = static_cast<PreferCode>(flags);
}

bool mrw::model::RailInfo::operator<(const mrw::model::RailInfo & other) const
{
	if (code == other.code)
	{
		return rail < other.rail;
	}
	return code < other.code;
}
