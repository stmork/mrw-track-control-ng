//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/signal.h"

using namespace mrw::model;

Signal::Signal(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const bool            is_main) :
	AssemblyPart(model_railway, element),
	Device(model_railway, element),
	direction(ModelRailway::boolean(element, "inZaehlrichtung")),
	is_main_signal(is_main)
{
}
