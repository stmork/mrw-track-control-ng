//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QDebug>

#include "model/modelrailway.h"
#include "model/signal.h"
#include "model/switchmodulereference.h"

using namespace mrw::model;
using namespace mrw::util;
using namespace mrw::can;

using LockState  = Device::LockState;
using Symbol     = Signal::Symbol;
using SignalType = Signal::SignalType;

const ConstantEnumerator<SignalType>  Signal::type_map
{
	CONSTANT(SignalType::MAIN_SIGNAL),
	CONSTANT(SignalType::DISTANT_SIGNAL),
	CONSTANT(SignalType::SHUNT_SIGNAL),
	CONSTANT(SignalType::MAIN_SHUNT_SIGNAL)
};

const ConstantEnumerator<Symbol>  Signal::symbol_map
{
	CONSTANT(Symbol::OFF),
	CONSTANT(Symbol::STOP),
	CONSTANT(Symbol::GO)
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

SignalType Signal::type() const
{
	return signal_type;
}

SignalAspect Signal::aspect() const
{
	return signal_aspect;
}

bool Signal::setAspect(const SignalAspect new_aspect)
{
	const Device * device   = dynamic_cast<const Device *>(this);
	const bool     modified = (signal_aspect != new_aspect) || (device->lock() != Device::LockState::LOCKED);

	signal_aspect = new_aspect;

	qDebug().noquote() << toString() << "modified:" << modified;
	return modified;
}

void Signal::link()
{
}

QString Signal::typeDescr() const
{
	QString type_descr;

	switch (signal_type)
	{
	case SignalType::MAIN_SIGNAL:
		type_descr = signal_direction ? " --O" : "O-- ";
		break;

	case SignalType::MAIN_SHUNT_SIGNAL:
		type_descr = signal_direction ? " -\\O" : "O\\- ";
		break;

	case SignalType::DISTANT_SIGNAL:
		type_descr = signal_direction ? " -o " : " o- ";
		break;

	case SignalType::SHUNT_SIGNAL:
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

QString Signal::get(const Symbol symbol)
{
	return symbol_map.get(symbol);
}

QString Signal::get(const Signal::SignalType type)
{
	return type_map.get(type);
}

void mrw::model::Signal::setSymbolCallback(std::function<Symbol ()> callback)
{
	symbol = callback;
}
