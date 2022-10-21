//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/mrwmodel.h"
#include "model/rail.h"

using namespace mrw::model;

Rail::Rail(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	RailPart(model_railway, element),
	is_main(  ModelRailway::boolean(element, "istHauptgleis")),
	is_branch(ModelRailway::boolean(element, "istAbzweig"))
{
}

void Rail::link()
{
	a = resolve("a");
	b = resolve("b");
}
