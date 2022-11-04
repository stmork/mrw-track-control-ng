//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/signal.h"
#include "model/switchmodulereference.h"

using namespace mrw::model;

Signal::Signal(
	ModelRailway     *    model_railway,
	const QDomElement  &  element,
	const SignalType      type) :
	AssemblyPart(model_railway, element),
	Device(model_railway, element),
	direction(ModelRailway::boolean(element, "inZaehlrichtung")),
	signal_type(type)
{
}

QString Signal::toString() const
{
	const SwitchModuleReference * module = dynamic_cast<const SwitchModuleReference *>(this);
	QString type_descr;

	switch (signal_type)
	{
	case MAIN_SIGNAL:
		type_descr = direction ? " --O" : "O-- ";
		break;

	case PRE_SIGNAL:
		type_descr = direction ? " -o " : " o- ";
		break;

	case SHUNT_SIGNAL:
		type_descr = direction ? " ]  " : "  [ ";
		break;
	}

	return QString("      %1    %2 : %3").
		arg(module != nullptr ? "F" : "L").
		arg(type_descr).arg(name());
}
