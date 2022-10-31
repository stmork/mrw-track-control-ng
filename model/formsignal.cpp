//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/mrwmodel.h"
#include "model/formsignal.h"
#include "model/switchmodule.h"

using namespace mrw::model;

FormSignal::FormSignal(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const bool            is_main) :
	Signal(model_railway, element, is_main),
	SwitchModuleReference(model_railway, element),
	inductors(model->value(element, "spulen"))
{
}

void FormSignal::link()
{
}
