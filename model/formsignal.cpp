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

const QString & FormSignal::name() const noexcept
{
	return part_name;
}

MrwMessage FormSignal::configMsg(const unsigned pin) const noexcept
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
		cmd = CFGSF2;
		break;

	case MAIN_SHUNT_SIGNAL:
		// This is not possible for form signals.
		break;
	}

	return configSwitchMsg(cmd, pin);
}

bool FormSignal::valid() const noexcept
{
	return (controller() != nullptr) && (module() != nullptr);
}

QString FormSignal::toString() const noexcept
{
	return QString("      F %1  %2   : [%3] %4 %5 %6").
		arg(valid()  ? "V" : "-").
		arg(symbol()).
		arg(unitNo(), 4, 16, QChar('0')).
		arg(name(), -10).
		arg(Device::get(lock()), -10).
		arg(MrwMessage::get(signal_aspect), -10);
}

bool FormSignal::isUnlockable() const noexcept
{
	return symbol() != Symbol::GO;
}
