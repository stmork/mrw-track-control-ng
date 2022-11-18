//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/signal.h"
#include "model/switchmodulereference.h"

using namespace mrw::model;
using namespace mrw::util;
using namespace mrw::can;

const ConstantEnumerator<SignalState> Signal::signal_constants
{
	CONSTANT(SIGNAL_OFF),
	CONSTANT(SIGNAL_HP0),
	CONSTANT(SIGNAL_HP1),
	CONSTANT(SIGNAL_HP2),
	CONSTANT(SIGNAL_VR0),
	CONSTANT(SIGNAL_VR1),
	CONSTANT(SIGNAL_VR2),
	CONSTANT(SIGNAL_SH0),
	CONSTANT(SIGNAL_SH1),
	CONSTANT(SIGNAL_TST)
};

Signal::Signal(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element,
	const SignalType      type) :
	AssemblyPart(model_railway, model_section, element),
	signal_direction(ModelRailway::boolean(element, "inZaehlrichtung")),
	signal_type(type)
{
}

void Signal::link()
{
}

QString Signal::symbol() const
{
	QString type_descr;

	switch (signal_type)
	{
	case MAIN_SIGNAL:
		type_descr = signal_direction ? " --O" : "O-- ";
		break;

	case MAIN_SIGNAL | SHUNT_SIGNAL:
		type_descr = signal_direction ? " -\\O" : "O\\- ";
		break;

	case DISTANT_SIGNAL:
		type_descr = signal_direction ? " -o " : " o- ";
		break;

	case SHUNT_SIGNAL:
		type_descr = signal_direction ? " ]  " : "  [ ";
		break;
	}
	return type_descr;
}

void Signal::findPair(const std::vector<Signal *> & section_signals)
{
	if (signal_type == MAIN_SIGNAL)
	{
		for (Signal * signal : section_signals)
		{
			if ((signal->signal_type == DISTANT_SIGNAL) &&
				(signal_direction == signal->signal_direction))
			{
				main_distant_pair = signal;
			}
		}
	}
}
