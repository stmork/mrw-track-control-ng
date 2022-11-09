//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/formsignal.h"
#include "model/switchmodule.h"

using namespace mrw::model;

FormSignal::FormSignal(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const SignalType      type) :
	Signal(model_railway, element, type),
	SwitchModuleReference(model_railway, element)
{
}

bool FormSignal::valid() const
{
	return (controller() != nullptr) && (module() != nullptr);
}

QString FormSignal::toString() const
{
	return QString("      F    %2  : %3").arg(symbol()).arg(name());
}
