//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include <model/modelrailway.h>
#include <model/formsignal.h>
#include <model/switchmodule.h>

using namespace mrw::can;
using namespace mrw::model;

FormSignal::FormSignal(
	ModelRailway     *    model_railway,
	Section       *       model_section,
	const QDomElement  &  element,
	const SignalType      type) :
	Signal(model_railway, model_section, element, type),
	SwitchModuleReference(model_railway, element, false)
{
}

const QString & FormSignal::name() const
{
	return part_name;
}

MrwMessage mrw::model::FormSignal::configMsg() const
{
	Command cmd = CMD_ILLEGAL;

	switch (type())
	{
	case MAIN_SIGNAL:
		switch (inductors())
		{
		case 2:
			cmd = CFGMF2;
			break;

		case 3:
			cmd = CFGMF3;
			break;
		}
		break;

	case DISTANT_SIGNAL:
		switch (inductors())
		{
		case 2:
			cmd = CFGPF2;
			break;

		case 3:
			cmd = CFGPF3;
			break;
		}
		break;

	case SHUNT_SIGNAL:
		// Configuring form shunt signals is not defined.
		break;

	case MAIN_SHUNT_SIGNAL:
		// This is not possible for form signals.
		break;
	}

	if (inductors() > 2)
	{
//		cmd = cmd + 1;
	}

	MrwMessage message = command(cmd);

	return message;
}

bool FormSignal::valid() const
{
	return (controller() != nullptr) && (module() != nullptr);
}

QString FormSignal::toString() const
{
	return QString("      F %1  %2   : [%3] %4 %5 %6").
		arg(valid()  ? "V" : "-").
		arg(symbol()).
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name(), -10).
		arg(Device::get(lock()), -10).
		arg(MrwMessage::get(signal_state), -10);
}

bool mrw::model::FormSignal::isUnlockable() const
{
	return symbol() != Symbol::GO;
}
