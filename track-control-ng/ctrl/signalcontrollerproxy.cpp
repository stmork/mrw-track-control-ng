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

using LockState = Device::LockState;

SignalControllerProxy::SignalControllerProxy(
	Section  *  parent_section,
	const bool  dir,
	QObject  *  parent) :
	SignalController(parent),
	statechart(nullptr),
	direction(dir),
	signal_section(parent_section)
{
	const std::vector<Signal *> & section_signals = signal_section->getSignals(dir);
	std::vector<RailPart *>       section_rails;
	QStringList                   list;

	// Find unfunctional rails.
	signal_section->parts<RailPart>(section_rails);
	signal_rail = section_rails.size() > 0 ? section_rails.front() : nullptr;

	Q_ASSERT(section_signals.size() > 0);

	// Sort by most significant signal first.
	base_signal = section_signals[0];

	for (Signal * signal : section_signals)
	{
		ControllerRegistry::instance().registerController(dynamic_cast<Device *>(signal), this);

		list << signal->partName();
		switch (signal->type())
		{
		case Signal::SHUNT_SIGNAL:
			shunt_signal   = signal;
			break;

		case Signal::DISTANT_SIGNAL:
			distant_signal = signal;
			break;

		case Signal::MAIN_SIGNAL:
			main_signal    = signal;
			break;

		case Signal::MAIN_SHUNT_SIGNAL:
			main_signal    = signal;
			shunt_signal   = signal;
			break;
		}
	}
	grouped_name = list.join("/");

	add(main_signal);
	add(distant_signal);
	add(shunt_signal);

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::clear,
		&statechart, &SignalControllerStatechart::clear,
		Qt::DirectConnection);
	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::start,
		&statechart, &SignalControllerStatechart::start,
		Qt::QueuedConnection);

	connect(
		this, &SignalControllerProxy::enable,
		&statechart, &SignalControllerStatechart::enable,
		Qt::QueuedConnection);
	connect(
		this, &SignalControllerProxy::disable,
		&statechart, &SignalControllerStatechart::disable,
		Qt::DirectConnection);
	connect(
		this, &SignalControllerProxy::failed,
		&statechart, &SignalControllerStatechart::failed,
		Qt::QueuedConnection);
	connect(
		this, &SignalControllerProxy::start,
		&statechart, &SignalControllerStatechart::start,
		Qt::QueuedConnection);

	connectMain();
	connectDistant();
	connectShunt();

	statechart_main.start(main_signal);
	statechart_distant.start(distant_signal, main_signal);
	statechart_shunt.start(shunt_signal, main_signal);

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

bool mrw::ctrl::SignalControllerProxy::isUnlocked() const
{
	return lock() == LockState::UNLOCKED;
}

Signal * SignalControllerProxy::mainSignal() const
{
	return main_signal;
}

void SignalControllerProxy::setCurved(const size_t curved_count)
{
	statechart_main.setCurved(curved_count);
}

void SignalControllerProxy::setDistantSignal(SignalControllerProxy * signal)
{
	statechart_distant.setMainController(signal);
}

void SignalControllerProxy::setSymbol(Signal::Symbol new_symbol)
{
	statechart.setSymbol(new_symbol);
}

void SignalControllerProxy::setState(SectionState new_state)
{
	is_tour = new_state == SectionState::TOUR;
}

void SignalControllerProxy::connectMain()
{
	connect(
		&statechart, &SignalControllerStatechart::turnMain,
		&statechart_main, &SignalStatechart::turn,
		Qt::QueuedConnection);
	connect(
		&statechart, &SignalControllerStatechart::cleared,
		&statechart_main, &SignalStatechart::clear,
		Qt::DirectConnection);
	connect(
		&statechart_main, &SignalStatechart::completed,
		&statechart, &SignalControllerStatechart::completedMain,
		Qt::QueuedConnection);
	connect(
		&statechart_main, &SignalStatechart::failed,
		&statechart, &SignalControllerStatechart::failed,
		Qt::QueuedConnection);
}

void SignalControllerProxy::connectDistant()
{
	connect(
		&statechart, &SignalControllerStatechart::turnDistant,
		&statechart_distant, &SignalStatechart::turn,
		Qt::QueuedConnection);
	connect(
		&statechart, &SignalControllerStatechart::cleared,
		&statechart_distant, &SignalStatechart::clear,
		Qt::DirectConnection);
	connect(
		&statechart_distant, &SignalStatechart::completed,
		&statechart, &SignalControllerStatechart::completedDistant,
		Qt::QueuedConnection);
	connect(
		&statechart_distant, &SignalStatechart::failed,
		&statechart, &SignalControllerStatechart::failed,
		Qt::QueuedConnection);
}

void SignalControllerProxy::connectShunt()
{
	connect(
		&statechart, &SignalControllerStatechart::turnShunt,
		&statechart_shunt, &SignalStatechart::turn,
		Qt::QueuedConnection);
	connect(
		&statechart, &SignalControllerStatechart::cleared,
		&statechart_shunt, &SignalStatechart::clear,
		Qt::DirectConnection);
	connect(
		&statechart_shunt, &SignalStatechart::completed,
		&statechart, &SignalControllerStatechart::completedShunt,
		Qt::QueuedConnection);
	connect(
		&statechart_shunt, &SignalStatechart::failed,
		&statechart, &SignalControllerStatechart::failed,
		Qt::QueuedConnection);
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
	return direction == region()->direction();
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
	if (railPart()->reserved())
	{
		return section()->occupation() ? SectionState::OCCUPIED : section()->state();
	}
	else
	{
		if (section()->anyReserved())
		{
			return SectionState::FREE;
		}
		else
		{
			return section()->occupation() ? SectionState::OCCUPIED : section()->state();
		}
	}
}

Device::LockState SignalControllerProxy::lock() const
{
	return lock_state;
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

RailPart * SignalControllerProxy::railPart() const
{
	return signal_rail;
}

Section * SignalControllerProxy::section() const
{
	return signal_section;
}

Signal::Symbol SignalControllerProxy::main() const
{
	return static_cast<Signal::Symbol>(statechart_main.getSignalState());
}

Signal::Symbol SignalControllerProxy::distant() const
{
	SignalControllerProxy * main_controller = statechart_distant.mainController();

	return main_controller != nullptr ?
		main_controller->main() :
		static_cast<Signal::Symbol>(statechart_distant.getSignalState());
}

Signal::Symbol SignalControllerProxy::shunt() const
{
	return static_cast<Signal::Symbol>(statechart_shunt.getSignalState());
}

bool SignalControllerProxy::process(const MrwMessage & message)
{
	Signal * signal = signal_map[message.unitNo()];

	const size_t processed =
		statechart_main.process(   signal, message) +
		statechart_distant.process(signal, message) +
		statechart_shunt.process(  signal, message);

	if (processed > 0)
	{
		emit update();
	}

	return processed;
}

void mrw::ctrl::SignalControllerProxy::restart()
{
	statechart.clear();
	emit start();
}

QString SignalControllerProxy::toString() const
{
	return QString("SCP %1%2%3           :        %5 %6").
		arg(hasMain()     ? 'M' : '-').
		arg(hasDistant()  ? 'D' : '-').
		arg(hasShunting() ? 'S' : '-').

		arg(grouped_name, -10).
		arg(Signal::get(static_cast<Signal::Symbol>(statechart.getSymbol())));
}

void SignalControllerProxy::inc()
{
	ControllerRegistry::instance().increase(this);
}

void SignalControllerProxy::dec()
{
	ControllerRegistry::instance().decrease(this);
}

bool SignalControllerProxy::hasMainSignal()
{
	return main_signal != nullptr;
}

bool SignalControllerProxy::isMain()
{
	return statechart_shunt.isCombined();
}

bool SignalControllerProxy::isTour()
{
	return is_tour;
}

void SignalControllerProxy::fail()
{
	qCritical().noquote() << String::red(" Signal turn failed!") << name();

	lock_state = LockState::FAIL;
	ControllerRegistry::instance().failed();
	emit update();
}

void SignalControllerProxy::pending()
{
	lock_state = LockState::PENDING;
	emit update();
}

void SignalControllerProxy::lock(const bool do_it)
{
	lock_state = do_it ? LockState::LOCKED : LockState::UNLOCKED;
	emit update();
}
