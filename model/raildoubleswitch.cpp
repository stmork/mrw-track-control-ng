//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/raildoubleswitch.h"

using namespace mrw::model;

RailDoubleSwitch::RailDoubleSwitch(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	AssemblyPart(model_railway, element),
	Device(element),
	SwitchModuleReference(model_railway, element),
	ad_branch(ModelRailway::boolean(element, "adIstAbzweig")),
	bc_branch(ModelRailway::boolean(element, "bcIstAbzweig"))
{
}

void RailDoubleSwitch::link()
{
	a = resolve("a");
	b = resolve("b");
	c = resolve("c");
	d = resolve("d");
}
