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
