//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <can/commands.h>
#include <ctrl/signalcontrollerproxy.h>

using namespace mrw::can;
using namespace mrw::ctrl;
using namespace mrw::model;

SignalControllerProxy::SignalControllerProxy(
	mrw::model::Section * parent_section,
	const bool            dir,
	QObject * parent) :
	SignalController(parent),
	section(parent_section),
	direction(dir)
{
	section->parts<Signal>(section_signals, [dir] (const Signal * input)
	{
		return dir == input->direction();
	});

	Q_ASSERT(section_signals.size() > 0);
}

QPoint SignalControllerProxy::point() const
{
	return section_signals[0]->position();
}

bool SignalControllerProxy::isDirection() const
{
	return direction;
}

bool SignalControllerProxy::hasShunting() const
{
	return std::any_of(section_signals.begin(), section_signals.end(), [] (Signal * input)
	{
		return input->type() == Signal::SHUNT_SIGNAL;
	});
}

bool SignalControllerProxy::hasDistant() const
{
	return std::any_of(section_signals.begin(), section_signals.end(), [] (Signal * input)
	{
		return input->type() == Signal::DISTANT_SIGNAL;
	});
}

bool SignalControllerProxy::hasMain() const
{
	return std::any_of(section_signals.begin(), section_signals.end(), [] (Signal * input)
	{
		return input->type() == Signal::MAIN_SIGNAL;
	});
}

QString SignalControllerProxy::name() const
{
	return "-";
}
