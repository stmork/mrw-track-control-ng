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
			shunt_signal = signal;
			break;
		}
	}
	grouped_name = list.join("/");

	add(main_signal);
	add(distant_signal);
	add(shunt_signal);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::inquire,
		&statechart, &SignalStatechart::start,
		Qt::QueuedConnection);

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

SignalController::TourState SignalControllerProxy::main() const
{
	return TourState::STOP;
}

bool SignalControllerProxy::process(const MrwMessage & message)
{
	Signal * device = signal_map[message.unitNo()];

	qDebug().noquote() << message << "  (signal)" << device->toString() << (ControllerRegistry::instance().contains(this) ? "yes" : "no");

	switch (message.response())
	{
	case MSG_QUEUED:
		statechart.queued();
		return true;

	case MSG_OK:
		switch (message.command())
		{
		case SETSGN:
			statechart.response();
			emit update();
			return true;

		default:
			// Intentionally do nothing.
			break;
		}
		break;

	default:
		qCritical().noquote() << "Error turning" << device->toString();
		statechart.fail();
		break;
	}
	return false;
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

bool SignalControllerProxy::hasMain()
{
	__METHOD__;

	qDebug().noquote() << " " << grouped_name;
	return main_signal != nullptr;
}

bool SignalControllerProxy::hasDistant()
{
	__METHOD__;

	qDebug().noquote() << " " << grouped_name;
	return distant_signal != nullptr;
}

bool SignalControllerProxy::hasShunt()
{
	__METHOD__;

	qDebug().noquote() << " " << grouped_name;
	return shunt_signal != nullptr;
}

enum Symbol : int
{
	OFF = -1,
	STOP = 0,
	GO = 1
};

void SignalControllerProxy::turnMainSignal(sc::integer symbol)
{
	__METHOD__;

	Q_ASSERT(main_signal != nullptr);

	SignalState state = SIGNAL_OFF;
	MrwMessage  message(main_signal->device()->command(SETSGN));

	switch (symbol)
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Symbol::STOP:
		state = SIGNAL_HP0;
		break;

	case Symbol::GO:
		state = SIGNAL_HP1;
		break;
	}

	message.append(state);
	ControllerRegistry::can()->write(message);

	qDebug().noquote() << message << "(signal)" << main_signal->toString() << (ControllerRegistry::instance().contains(this) ? "yes" : "no");
}

void SignalControllerProxy::turnDistantSignal(sc::integer symbol)
{
	__METHOD__;

	Q_ASSERT(distant_signal != nullptr);

	SignalState state = SIGNAL_OFF;
	MrwMessage  message(distant_signal->device()->command(SETSGN));

	switch (symbol)
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Symbol::STOP:
		state = SIGNAL_VR0;
		break;

	case Symbol::GO:
		state = SIGNAL_VR1;
		break;
	}


	message.append(state);
	ControllerRegistry::can()->write(message);

	qDebug().noquote() << message << "(signal)" << distant_signal->toString() << (ControllerRegistry::instance().contains(this) ? "yes" : "no");
}

void SignalControllerProxy::turnShuntSignal(sc::integer symbol)
{
	__METHOD__;

	Q_ASSERT(shunt_signal != nullptr);

	SignalState state = SIGNAL_OFF;
	MrwMessage  message(shunt_signal->device()->command(SETSGN));

	switch (symbol)
	{
	default:
		state = SIGNAL_OFF;
		break;

	case Symbol::STOP:
		state = SIGNAL_SH0;
		break;

	case Symbol::GO:
		state = SIGNAL_SH1;
		break;
	}

	message.append(state);
	ControllerRegistry::can()->write(message);

	qDebug().noquote() << message << "(signal)" << shunt_signal->toString() << (ControllerRegistry::instance().contains(this) ? "yes" : "no");
}

SignalController::TourState SignalControllerProxy::distant() const
{
	return TourState::STOP;
}

SignalController::TourState SignalControllerProxy::shunt() const
{
	return TourState::STOP;
}
