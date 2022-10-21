//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/mrwmodel.h"
#include "model/signal.h"

using namespace mrw::model;

Signal::Signal(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const bool            is_main) :
	RailPart(model_railway, element),
	direction(ModelRailway::boolean(element, "inZaehlrichtung")),
	unit_no(ModelRailway::value(element, "unit_no")),
	is_main_signal(is_main)
{
}
