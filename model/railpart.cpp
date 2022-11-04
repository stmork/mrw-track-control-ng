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
	const QDomElement  &  element) :
	AssemblyPart(model_railway, element),
	a_in_dir(ModelRailway::boolean(element, "aInZaehlrichtung"))
{
}

bool RailPart::contains(const RailPart * rail, const bool dir) const
{
	const std::set<RailPart *> & rails = advance(!dir);

	return rails.find((RailPart *)rail) != rails.end();
}

RailPart * RailPart::resolve(const char * attr) const
{
	const QString & value = reference.attribute(attr, "");

	return dynamic_cast<RailPart *>(AssemblyPart::resolve(model, value));
}

std::set<RailPart *> & RailPart::advance(const bool dir)
{
	return dir ? rail_forward : rail_backward;
}

const std::set<RailPart *> & RailPart::advance(const bool dir) const
{
	return dir ? rail_forward : rail_backward;
}
