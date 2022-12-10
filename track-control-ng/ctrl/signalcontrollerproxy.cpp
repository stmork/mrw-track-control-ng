//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <util/method.h>
#include <can/commands.h>
#include <model/formsignal.h>
#include <ctrl/signalcontrollerproxy.h>
#include <ctrl/controllerregistry.h>
#include <statecharts/timerservice.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::statechart;

SignalControllerProxy::SignalControllerProxy(
	Section  *  parent_section,
	const bool  dir,
	QObject  *  parent) :
	SignalController(parent),
	statechart(nullptr),
	direction(dir),
	section(parent_section)
{
	std::vector<Signal *> section_signals;
	QStringList           list;

	section->parts<Signal>(section_signals, [dir] (const Signal * input)
	{
		return dir == input->direction();
	});

	Q_ASSERT(section_signals.size() > 0);

	// Sort by most significant signal first.
	std::sort(section_signals.begin(), section_signals.end(), Signal::less);
	base_signal = section_signals[0];

	for (Signal * signal : section_signals)
	{
		ControllerRegistry::instance().registerController(dynamic_cast<Device *>(signal), this);

		list << signal->partName();
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
			shunt_signal = nullptr;
			break;
		}
	}
	grouped_name = list.join("/");

	add(main_signal);
	add(distant_signal);
	add(shunt_signal);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::inquire,
		&statechart, &SignalControllerStatechart::start,
		Qt::QueuedConnection);

	connect(
		&statechart, &SignalControllerStatechart::turnMain,
		&statechart_main, &SignalStatechart::turn,
		Qt::QueuedConnection);
	connect(
		&statechart_main, &SignalStatechart::completed,
		&statechart, &SignalControllerStatechart::completedMain,
		Qt::QueuedConnection);
	connect(
		&statechart_main, &SignalStatechart::failed,
		&statechart, &SignalControllerStatechart::failed,
		Qt::QueuedConnection);

	connect(
		&statechart, &SignalControllerStatechart::turnDistant,
		&statechart_distant, &SignalStatechart::turn,
		Qt::QueuedConnection);
	connect(
		&statechart_distant, &SignalStatechart::completed,
		&statechart, &SignalControllerStatechart::completedDistant,
		Qt::QueuedConnection);
	connect(
		&statechart_distant, &SignalStatechart::failed,
		&statechart, &SignalControllerStatechart::failed,
		Qt::QueuedConnection);

	connect(
		&statechart, &SignalControllerStatechart::turnShunt,
		&statechart_shunt, &SignalStatechart::turn,
		Qt::QueuedConnection);
	connect(
		&statechart_shunt, &SignalStatechart::completed,
		&statechart, &SignalControllerStatechart::completedShunt,
		Qt::QueuedConnection);
	connect(
		&statechart_shunt, &SignalStatechart::failed,
		&statechart, &SignalControllerStatechart::failed,
		Qt::QueuedConnection);

	connect(
		this, &SignalControllerProxy::enable,
		&statechart, &SignalControllerStatechart::enable,
		Qt::QueuedConnection);
	connect(
		this, &SignalControllerProxy::disable,
		&statechart, &SignalControllerStatechart::disable,
		Qt::QueuedConnection);

	statechart_main.start(main_signal);
	statechart_distant.start(distant_signal);
	statechart_shunt.start(shunt_signal);

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);
	statechart.enter();
}

SignalControllerProxy::~SignalControllerProxy()
{
	statechart.exit();

	ControllerRegistry::instance().unregisterController(
		dynamic_cast<Device *>(main_signal));
	ControllerRegistry::instance().unregisterController(
		dynamic_cast<Device *>(distant_signal));
	ControllerRegistry::instance().unregisterController(
		dynamic_cast<Device *>(shunt_signal));
}

void SignalControllerProxy::add(Signal * signal)
{
	if (signal != nullptr)
	{
		Device * device = signal->device();

		signal_map.emplace(device->unitNo(), signal);
	}
}

QString SignalControllerProxy::name() const
{
	return grouped_name;
}

float SignalControllerProxy::extensions() const
{
	return base_signal->extension();
}

bool SignalControllerProxy::isDirection() const
{
	return direction == section->region()->direction();
}

bool SignalControllerProxy::isExpandable() const
{
	return true;
}

Position * SignalControllerProxy::position() const
{
	return base_signal;
}

SectionState SignalControllerProxy::state() const
{
	return section->occupation() ? SectionState::OCCUPIED : section->state();
}

Device::LockState SignalControllerProxy::lock() const
{
	Device * device = dynamic_cast<Device *>(base_signal);

	Q_ASSERT(device != nullptr);
	return device->lock();
}

Position::Bending SignalControllerProxy::bending() const
{
	return base_signal->bending();
}

bool SignalControllerProxy::hasMain() const
{
	return main_signal != nullptr;
}

bool SignalControllerProxy::hasDistant() const
{
	return distant_signal != nullptr;
}

bool SignalControllerProxy::hasShunting() const
{
	return shunt_signal != nullptr;
}

Signal::Symbol SignalControllerProxy::main() const
{
	return static_cast<Signal::Symbol>(statechart_main.getSignalState());
}

Signal::Symbol SignalControllerProxy::distant() const
{
	return static_cast<Signal::Symbol>(statechart_distant.getSignalState());
}

Signal::Symbol SignalControllerProxy::shunt() const
{
	return static_cast<Signal::Symbol>(statechart_shunt.getSignalState());
}

bool SignalControllerProxy::process(const MrwMessage & message)
{
	Signal * signal = signal_map[message.unitNo()];

	const bool processed =
		statechart_main.process(   signal, message) ||
		statechart_distant.process(signal, message) ||
		statechart_shunt.process(  signal, message);

	if (processed)
	{
		emit update();
	}

	return processed;
}

QString SignalControllerProxy::toString() const
{
	return *base_signal;
}

void SignalControllerProxy::inc()
{
	ControllerRegistry::instance().increase(this);
}

void SignalControllerProxy::dec()
{
	ControllerRegistry::instance().decrease(this);
}

bool mrw::ctrl::SignalControllerProxy::hasMain()
{
	return main_signal != nullptr;
}

void mrw::ctrl::SignalControllerProxy::prepare()
{
	__METHOD__;

	// TODO: Compute real signal symbols from what the route/tour wants.
}

void SignalControllerProxy::fail()
{
	__METHOD__;

	// TODO: Implement
	qCritical().noquote() << String::red(" Signal turn failed!");
}

void SignalControllerProxy::pending()
{
	// TODO: Implement
	qDebug(" Pending");
}

void SignalControllerProxy::lock(const bool do_it)
{
	__METHOD__;

	// TODO: Implement
	qDebug(do_it ? " Lock" : " Unlock");
}
