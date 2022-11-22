//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <can/commands.h>
#include <ctrl/signalcontrollerproxy.h>
#include <model/region.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;

SignalControllerProxy::SignalControllerProxy(
	Section  *  parent_section,
	const bool  dir,
	QObject  *  parent) :
	SignalController(parent),
	direction(dir),
	section(parent_section)
{
	std::vector<Signal *> section_signals;

	section->parts<Signal>(section_signals, [dir] (const Signal * input)
	{
		return dir == input->direction();
	});

	Q_ASSERT(section_signals.size() > 0);

	base_signal = section_signals[0];

	for (Signal * signal : section_signals)
	{
		switch (signal->type())
		{
		case Signal::SHUNT_SIGNAL:
			shunt_signal = signal;
			break;

		case Signal::DISTANT_SIGNAL:
			distant_signal = signal;
			break;

		case Signal::MAIN_SIGNAL:
			main_signal = signal;
			break;

		case Signal::MAIN_SHUNT_SIGNAL:
			main_signal = signal;
			shunt_signal = signal;
			break;
		}
	}
}

bool SignalControllerProxy::isDirection() const
{
	return direction == section->region()->direction();
}

bool SignalControllerProxy::hasShunting() const
{
	return shunt_signal != nullptr;
}

bool SignalControllerProxy::hasDistant() const
{
	return distant_signal != nullptr;
}

bool SignalControllerProxy::hasMain() const
{
	return main_signal != nullptr;
}

QString SignalControllerProxy::name() const
{
	return base_signal->partName();
}

SignalController::TourState SignalControllerProxy::main() const
{
	return TourState::STOP;
}

Device::LockState mrw::ctrl::SignalControllerProxy::lock() const
{
	Device * device = dynamic_cast<Device *>(base_signal);

	Q_ASSERT(device != nullptr);
	return device->lock();
}

unsigned mrw::ctrl::SignalControllerProxy::extensions() const
{
	return base_signal->extend();
}

SignalController::TourState SignalControllerProxy::distant() const
{
	return TourState::STOP;
}

SignalController::TourState SignalControllerProxy::shunt() const
{
	return TourState::STOP;
}
