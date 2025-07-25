//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>
#include <QDebug>

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

using Bending    = Position::Bending;
using LockState  = Device::LockState;
using Symbol     = Signal::Symbol;
using SignalType = Signal::SignalType;

#define not_VERBOSE

SignalControllerProxy::SignalControllerProxy(
	Section  *  parent_section,
	const bool  dir,
	QObject  *  parent) :
	SignalController(parent),
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
		case SignalType::SHUNT_SIGNAL:
			shunt_signal   = signal;
			shunt_signal->setSymbolCallback(std::bind(&SignalControllerProxy::shunt, this));
			break;

		case SignalType::DISTANT_SIGNAL:
			distant_signal = signal;
			distant_signal->setSymbolCallback(std::bind(&SignalControllerProxy::distant, this));
			break;

		case SignalType::MAIN_SIGNAL:
			main_signal    = signal;
			main_signal->setSymbolCallback(std::bind(&SignalControllerProxy::main, this));
			break;

		case SignalType::MAIN_SHUNT_SIGNAL:
			main_signal    = signal;
			shunt_signal   = signal;
			main_signal->setSymbolCallback(std::bind(&SignalControllerProxy::main, this));
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

void SignalControllerProxy::connectStatechart()
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
	connect(
		&statechart, &SignalControllerStatechart::entered, [&]()
	{
//		QCoreApplication::processEvents();

		qCDebug(log).noquote() << toString() << "Inquiry started.";
	});
	connect(
		&statechart, &SignalControllerStatechart::started, [&]()
	{
		qCDebug(log).noquote() << toString() << "Inquiry completed.";
	});
}

void SignalControllerProxy::initStatechart()
{
	statechart_main.start(main_signal);
	statechart_distant.start(distant_signal, main_signal);
	statechart_shunt.start(shunt_signal, main_signal);

	statechart.setTimerService(TimerService::instance());
	statechart.setOperationCallback(*this);

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
	return section()->state() == SectionState::PASSED;
}

Signal * SignalControllerProxy::mainSignal() const
{
	return main_signal;
}

void SignalControllerProxy::setCurvedCount(const size_t curved)
{
	statechart_main.setCurveCount(curved);
}

void SignalControllerProxy::setDistantSignal(SignalControllerProxy * signal)
{
	statechart_distant.setMainController(signal);
}

void SignalControllerProxy::setSymbol(const Symbol new_symbol)
{
	statechart.setSymbol(sc::integer(new_symbol));
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

const QString & SignalControllerProxy::name() const noexcept
{
#ifdef VERBOSE
	return grouped_name;
#else
	return section()->getSignals(direction).front()->partName();
#endif
}

float SignalControllerProxy::extensions() const noexcept
{
	return base_signal->extension();
}

bool SignalControllerProxy::isDirection() const noexcept
{
	return direction == region()->direction();
}

Position * SignalControllerProxy::position() const noexcept
{
	return base_signal;
}

SectionState SignalControllerProxy::state() const noexcept
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

LockState SignalControllerProxy::lock() const noexcept
{
	return lock_state;
}

Bending SignalControllerProxy::bending() const noexcept
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

RailPart * SignalControllerProxy::railPart() const noexcept
{
	return signal_rail;
}

Section * SignalControllerProxy::section() const noexcept
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

void SignalControllerProxy::restart()
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
	qCCritical(log).noquote() << String::red(" Signal turn failed!") << name();

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
	qCDebug(log).noquote() << String::bold("Pend:") << *this;
#endif
}

void SignalControllerProxy::lock(const bool do_it)
{
	setLockState(do_it ? LockState::LOCKED : LockState::UNLOCKED);
	emit update();

#ifdef VERBOSE
	qCDebug(log).noquote() << String::bold("Lock:") << *this;
#endif
}
