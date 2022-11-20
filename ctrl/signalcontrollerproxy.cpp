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
	section(parent_section),
	direction(dir)
{
	std::vector<Signal *> section_signals;

	section->parts<Signal>(section_signals, [dir] (const Signal * input)
	{
		return dir == input->direction();
	});

	Q_ASSERT(section_signals.size() > 0);
	signal_name = section_signals[0]->partName();
	position    = section_signals[0]->position();

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

QPoint SignalControllerProxy::point() const
{
	return position;
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
	return signal_name;
}

SignalController::TourState SignalControllerProxy::main() const
{
	return TourState::STOP;
}

SignalController::TourState SignalControllerProxy::distant() const
{
	return TourState::STOP;
}

SignalController::TourState SignalControllerProxy::shunt() const
{
	return TourState::STOP;
}
