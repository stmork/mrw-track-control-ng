//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <model/modelrailway.h>
#include <model/crossing.h>

using namespace mrw::model;

Crossing::Crossing(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element) :
	AssemblyPart(model_railway, model_section, element),
	Device(model_railway, element)
{
	sections.emplace_back(model_section);
	model_railway->add(this);
}
