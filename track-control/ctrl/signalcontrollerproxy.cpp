//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <util/method.h>
#include <util/stringutil.h>
#include <can/commands.h>
#include <model/lightsignal.h>
#include <model/formsignal.h>
#include <ctrl/signalcontrollerproxy.h>
#include <ctrl/controllerregistry.h>
#include <statecharts/timerservice.h>

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::statechart;

using Bending   = Position::Bending;
using LockState = Device::LockState;
using Symbol    = Signal::Symbol;

#define not_VERBOSE

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

	connectMain();
	connectDistant();
	connectShunt();
	connectStatechart();

	initStatechart();
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

void mrw::ctrl::SignalControllerProxy::connectStatechart()
{
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
		Qt::DirectConnection);
	connect(
		this, &SignalControllerProxy::extend,
		&statechart, &SignalControllerStatechart::extend,
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
}

void mrw::ctrl::SignalControllerProxy::initStatechart()
{
	statechart_main.start(main_signal);
	statechart_distant.start(distant_signal, main_signal);
	statechart_shunt.start(shunt_signal, main_signal);

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);

	Q_ASSERT(statechart.check());
	statechart.enter();
}

void SignalControllerProxy::add(Signal * signal)
{
	Device * device = dynamic_cast<Device *>(signal);

	if (device != nullptr)
	{
		signal_map.emplace(device->unitNo(), signal);
	}
}

void SignalControllerProxy::setLockState(LockState state)
{
	lock_state = state;
	for (auto it : signal_map)
	{
		Device * device = dynamic_cast<Device *>(it.second);

		device->setLock(state);
	}
}

/*************************************************************************
**                                                                      **
**       Service methods                                                **
**                                                                      **
*************************************************************************/

bool SignalControllerProxy::isUnlocked() const
{
	return lock_state == LockState::UNLOCKED;
}

bool SignalControllerProxy::isPassed() const
{
	return section()->state() == PASSED;
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

/*************************************************************************
**                                                                      **
**       Implementation of BaseController                               **
**                                                                      **
*************************************************************************/

QString SignalControllerProxy::name() const
{
#ifdef VERBOSE
	return grouped_name;
#else
	return section()->getSignals(direction).front()->partName();
#endif
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
		return section()->occupation() ?
			SectionState::OCCUPIED :
			section()->state();
	}
	else
	{
		if (section()->anyReserved())
		{
			return SectionState::FREE;
		}
		else
		{
			return section()->occupation() ?
				SectionState::OCCUPIED :
				section()->state();
		}
	}
}

LockState SignalControllerProxy::lock() const
{
	return lock_state;
}

Bending SignalControllerProxy::bending() const
{
	return base_signal->bending();
}

/*************************************************************************
**                                                                      **
**       Implementation of SignalController                             **
**                                                                      **
*************************************************************************/

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

Symbol SignalControllerProxy::main() const
{
	return static_cast<Signal::Symbol>(statechart_main.getSymbol());
}

Symbol SignalControllerProxy::distant() const
{
	return statechart_distant.getPreparedSymbol();
}

Symbol SignalControllerProxy::shunt() const
{
	return static_cast<Signal::Symbol>(statechart_shunt.getSymbol());
}

/*************************************************************************
**                                                                      **
**       Implementation of RailPartInfo                                 **
**                                                                      **
*************************************************************************/

RailPart * SignalControllerProxy::railPart() const
{
	return signal_rail;
}

Section * SignalControllerProxy::section() const
{
	return signal_section;
}

/*************************************************************************
**                                                                      **
**       Implementation of ControllerRegistrand                         **
**                                                                      **
*************************************************************************/

bool SignalControllerProxy::process(const MrwMessage & message)
{
	Signal    *    signal    = signal_map[message.unitNo()];
	const size_t   processed =
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
	Symbol symbol = (Symbol)statechart.getSymbol();

	return QString("SCP %1%2%3           :        %4 %5 %6 - %7 %8 %9").
		arg(hasMain()     ? 'M' : '-').
		arg(hasDistant()  ? 'D' : '-').
		arg(hasShunting() ? 'S' : '-').

		arg(grouped_name, -10).
		arg(Signal::get(symbol), -4).
		arg(Device::get(lock_state), -8).

		arg(Signal::get(main()),    -4).
		arg(Signal::get(distant()), -4).
		arg(Signal::get(shunt()),   -4);
}

/*************************************************************************
**                                                                      **
**       Implementation of OperationCallback                            **
**                                                                      **
*************************************************************************/

void SignalControllerProxy::inc()
{
	increase();
}

void SignalControllerProxy::dec()
{
	decrease();
}

bool SignalControllerProxy::hasMainSignal()
{
	return main_signal != nullptr;
}

bool SignalControllerProxy::isMainAndShunt()
{
	return statechart_shunt.isCombined();
}

bool SignalControllerProxy::isLightSignal()
{
	return dynamic_cast<LightSignal *>(distant_signal) != nullptr;
}

bool SignalControllerProxy::isTour()
{
	return is_tour;
}

void SignalControllerProxy::fail()
{
	qCritical().noquote() << String::red(" Signal turn failed!") << name();

	statechart_main.fail();
	statechart_distant.fail();
	statechart_shunt.fail();

	setLockState(LockState::FAIL);
	ControllerRegistry::instance().failed();
	emit update();
}

void SignalControllerProxy::pending()
{
	setLockState(LockState::PENDING);
	emit update();

#ifdef VERBOSE
	qDebug().noquote() << String::bold("Pend:") << *this;
#endif
}

void SignalControllerProxy::lock(const bool do_it)
{
	setLockState(do_it ? LockState::LOCKED : LockState::UNLOCKED);
	emit update();

#ifdef VERBOSE
	qDebug().noquote() << String::bold("Lock:") << *this;
#endif
}
