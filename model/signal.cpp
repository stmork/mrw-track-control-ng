//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "model/modelrailway.h"
#include "model/signal.h"
#include "model/switchmodulereference.h"

using namespace mrw::model;
using namespace mrw::util;
using namespace mrw::can;

using LockState  = Device::LockState;
using Symbol     = Signal::Symbol;
using SignalType = Signal::SignalType;

const ConstantEnumerator<SignalAspect> Signal::signal_constants
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

const ConstantEnumerator<SignalType>  Signal::type_map
{
	CONSTANT(MAIN_SIGNAL),
	CONSTANT(DISTANT_SIGNAL),
	CONSTANT(SHUNT_SIGNAL),
	CONSTANT(MAIN_SHUNT_SIGNAL)
};

const ConstantEnumerator<Symbol>  Signal::symbol_map
{
	CONSTANT(OFF),
	CONSTANT(STOP),
	CONSTANT(GO)
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

bool Signal::direction() const
{
	return signal_direction;
}

Signal::SignalType Signal::type() const
{
	return signal_type;
}

SignalAspect Signal::aspect() const
{
	return signal_aspect;
}

void Signal::setAspect(const SignalAspect new_aspect)
{
	signal_aspect = new_aspect;
	qDebug().noquote() << toString();
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

QString Signal::key() const
{
	return section()->region()->name() + partName();
}

bool Signal::less(Signal * left, Signal * right)
{
	if (left->direction() == right->direction())
	{
		return left->type() > right->type();
	}
	return left->direction() > right->direction();
}

QString Signal::get(const Signal::Symbol & symbol)
{
	return symbol_map.get(symbol);
}

QString Signal::get(const Signal::SignalType & type)
{
	return type_map.get(type);
}
