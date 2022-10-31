//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/mrwmodel.h"
#include "model/railswitch.h"

using namespace mrw::model;

RailSwitch::RailSwitch(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	RailPart(model_railway, element),
	SwitchModuleReference(model_railway, element),
	left_branch( ModelRailway::boolean(element, "bIstAbzweig", false)),
	right_branch(ModelRailway::boolean(element, "cIstAbzweig", false)),
	left_prio(   ModelRailway::boolean(element, "bIstBevorzugt", false)),
	right_prio(  ModelRailway::boolean(element, "cIstBevorzugt", false))
{
}

void RailSwitch::link()
{
	a = resolve("a");
	b = resolve("b");
	c = resolve("c");
}
