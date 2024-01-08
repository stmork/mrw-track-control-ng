//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QMetaMethod>
#include <QCoreApplication>

#include <util/method.h>
#include <util/stringutil.h>
#include <statecharts/timerservice.h>
#include <ctrl/controllerregistry.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/sectioncontroller.h>
#include <ctrl/signalcontrollerproxy.h>

#include "controlledroute.h"
#include "beermodeservice.h"

using namespace mrw::util;
using namespace mrw::statechart;
using namespace mrw::model;
using namespace mrw::ctrl;

using LockState = Device::LockState;
using Symbol    = Signal::Symbol;

ControlledRoute::ControlledRoute(
	const bool           dir,
	const SectionState   wanted_state,
	RailPart      *      first,
	QObject       *      parent) :
	Route(dir, wanted_state, first, parent)
{
	rename();
	list_item.setData(USER_ROLE, QVariant::fromValue(this));

	connect(
		this, &ControlledRoute::completed,
		&statechart, &RouteStatechart::completed,
		Qt::QueuedConnection);
	connect(
		this, &ControlledRoute::disable,
		&statechart, &RouteStatechart::disable,
		Qt::QueuedConnection);
	connect(
		this, &ControlledRoute::turn,
		&statechart, &RouteStatechart::turn,
		Qt::QueuedConnection);
	connect(
		&statechart, &RouteStatechart::finished,
		this, &ControlledRoute::finished,
		Qt::QueuedConnection);
	connect(
		&statechart, &RouteStatechart::activated,
		this, &ControlledRoute::dump,
		Qt::QueuedConnection);

	statechart.setTimerService(TimerService::instance());
	statechart.setOperationCallback(*this);

	Q_ASSERT(statechart.check());
	statechart.enter();
}

ControlledRoute::~ControlledRoute()
{
	std::vector<BatchParticipant *> participants;

	ControllerRegistry::instance().collect<BatchParticipant>(participants);
	for (BatchParticipant * part : participants)
	{
		if (part->batch() == this)
		{
			qWarning().noquote() << "Batch participant not deregistered:" << part->name();
			part->setBatch(nullptr);
		}
	}
	Batch::dump();

	statechart.disable();
	statechart.exit();
}

/*************************************************************************
**                                                                      **
**       Preparing extension of route                                   **
**                                                                      **
*************************************************************************/

void ControlledRoute::prepareRoute()
{
	__METHOD__;

	rename();
}

bool ControlledRoute::prepare()
{
	const bool success = Route::prepare();

	if (success)
	{
		prepareSections();
		prepareTrack();
		prepareSignals();
	}
	return success;
}

void ControlledRoute::prepareSections()
{
	__METHOD__;

	std::vector<SectionController *> controllers;

	collectSectionControllers(controllers);
	for (SectionController * controller : controllers)
	{
		controller->setAutoOff(true);
		controller->setAutoUnlock(auto_unblock);

		if (controller->section()->lock() != LockState::LOCKED)
		{
			connectSectionController(controller);
		}
	}
}

void ControlledRoute::prepareTrack()
{
	__METHOD__;

	for (auto it = track.rbegin(); *it != last_valid_part; ++it)
	{
		RailPart    *    part      = *it;
		Device     *     device    = dynamic_cast<Device *>(part);
		BaseController * part_ctrl =
			ControllerRegistry::instance().find<BaseController>(device);

		RegularSwitchControllerProxy   *   rs  =
			dynamic_cast<RegularSwitchControllerProxy *>(part_ctrl);
		DoubleCrossSwitchControllerProxy * dcs =
			dynamic_cast<DoubleCrossSwitchControllerProxy *>(part_ctrl);

		if (rs != nullptr)
		{
			connect(
				rs, &RegularSwitchControllerProxy::stop,
				&statechart, &RouteStatechart::failed,
				Qt::UniqueConnection);
			rs->setBatch(this);
		}

		if (dcs != nullptr)
		{
			connect(
				dcs, &DoubleCrossSwitchControllerProxy::stop,
				&statechart, &RouteStatechart::failed,
				Qt::UniqueConnection);
			dcs->setBatch(this);
		}
		qDebug().noquote() << *part;
	}
}

void ControlledRoute::prepareSignals()
{
	__METHOD__;

	SignalControllerProxy * main_controller = nullptr;
	SectionController   *   next_ctrl       = nullptr;
	size_t                  curved          = 0;

	for (auto it = sections.rbegin(); it != sections.rend(); ++it)
	{
		Section        *        section      = *it;
		SectionController   *   section_ctrl = getSectionController(section);
		SignalControllerProxy * signal_ctrl  = getSignalController(section);
		std::vector<RailPart *> rails;

		section->parts<RailPart>(rails, &ControlledRoute::isCurved);
		curved += rails.size();

		if (signal_ctrl != nullptr)
		{
			// OK, there is a signal...
			if (signal_ctrl->hasDistant())
			{
				// OK, a distant signal has to reflect the main signals state.
				signal_ctrl->setDistantSignal(main_controller);
			}

			if (signal_ctrl->hasMain())
			{
				//  OK, a main signal starts a new block.
				main_controller = signal_ctrl;
				signal_ctrl->setCurvedCount(curved);
				curved = 0;
			}

			signal_ctrl->setState(state);
			signal_ctrl->setSymbol(section != this->last_section ?
				Symbol::GO :
				Symbol::STOP);
			signal_ctrl->setBatch(this);
			qDebug().noquote() << *signal_ctrl;
		}

		section_ctrl->nextController(next_ctrl);
		next_ctrl = section_ctrl;
	}
}

void ControlledRoute::prepareFlank()
{
	__METHOD__;

	Route::prepareFlank();
}

void ControlledRoute::connectSectionController(SectionController * controller)
{
	connect(
		controller, &SectionController::entered,
		this, &ControlledRoute::entered,
		Qt::UniqueConnection);
	connect(
		controller, &SectionController::leaving,
		this, &ControlledRoute::leaving,
		Qt::UniqueConnection);
	connect(
		controller, &SectionController::left,
		this, &ControlledRoute::left,
		Qt::UniqueConnection);
	connect(
		controller, &SectionController::tryUnblock,
		this, &ControlledRoute::tryUnblock,
		Qt::UniqueConnection);
	connect(
		controller, &SectionController::unregister,
		this, qOverload<>(&ControlledRoute::unregister),
		Qt::UniqueConnection);
	connect(
		controller, &SectionController::stop,
		&statechart, &RouteStatechart::failed,
		Qt::UniqueConnection);

	controller->setBatch(this);
}

void ControlledRoute::rename()
{
	QString name = QString("%1 %2").
		arg(state == SectionState::TOUR ? "F" : "R").
		arg(direction ? "re." : "li.");

	if (sections.size() > 0)
	{
		name += ": ";
		name += sections.front()->name();
		if (sections.front() != sections.back())
		{
			name += " => ";
			name += sections.back()->name();
		}
	}
	list_item.setText(name);
}

/*************************************************************************
**                                                                      **
**       Releasing parts of route                                       **
**                                                                      **
*************************************************************************/

/**
 * This slot is only a marker that a train has entered a mrw::model::Section.
 */
void ControlledRoute::entered()
{
	SectionController * controller = dynamic_cast<SectionController *>(QObject::sender());

	qDebug().noquote() << "Entered:    " << *controller;
}

void ControlledRoute::leaving()
{
	__METHOD__;

	SectionController   *   section_ctrl  = dynamic_cast<SectionController *>(QObject::sender());
	SignalControllerProxy * signal_ctrl   = getSignalController(section_ctrl->section());
	RouteBatch       *      disable_batch = new RouteBatch(signal_ctrl != nullptr, auto_unblock, this);

	section_ctrl->setBatch(disable_batch);
	if (signal_ctrl != nullptr)
	{
		signal_ctrl->setBatch(disable_batch);
	}

	connect (
		disable_batch, &RouteBatch::unlock,
		section_ctrl,  &SectionController::unlock,
		Qt::QueuedConnection);
	connect (
		disable_batch, &RouteBatch::tryUnblock,
		section_ctrl,  &SectionController::tryUnblock,
		Qt::QueuedConnection);
}

/**
 * This slot marks a section left. All signals inside has to be turned to STOP.
 */
void ControlledRoute::left()
{
	__METHOD__;

	SectionController   *   section_ctrl = dynamic_cast<SectionController *>(QObject::sender());
	SignalControllerProxy * signal_ctrl  = getSignalController(section_ctrl->section());

	if (signal_ctrl != nullptr)
	{
		signal_ctrl->disable();
		qDebug().noquote() << "Left sig.:  " << *signal_ctrl;
	}
	qDebug().noquote() << "Left sec.:  " << *section_ctrl;
}

/**
 * This slot method tries to unblock all sections back until the last main
 * signal. Its a callback from the SectionStatechart when auto_unblock is
 * disabled causing the left sections marked as PASSED. Since the signals
 * are already turnd to stop (@see left()) the sections needs only to be
 * unregistered.
 */
void ControlledRoute::tryUnblock()
{
	tryUnblockCtrl(dynamic_cast<SectionController *>(QObject::sender()));
}

void ControlledRoute::tryUnblockCtrl(SectionController * section_ctrl)
{
	__METHOD__;

	Section        *        section     = section_ctrl->section();
	SignalControllerProxy * signal_ctrl = getSignalController(section);
	Signal         *        main_signal = signal_ctrl != nullptr ? signal_ctrl->mainSignal() : nullptr;

	if (main_signal != nullptr)
	{
		Section * first;

		qDebug().noquote() << "Try unblock: until >>>" << *main_signal;
		do
		{
			first = sections.front();

			qDebug().noquote() << "Try unblock:" << *first;
			unregister(first);
		}
		while (first != section);
	}
	else
	{
		qDebug().noquote() << "Try unblock: no signal";
	}
	qDebug().noquote() << "Try unblock:" << countAllocatedSections() << "sections left";

	rename();
	finalize();
}

void ControlledRoute::unregister()
{
	__METHOD__;

	SectionController * section_ctrl =
		dynamic_cast<SectionController *>(QObject::sender());

	unregister(section_ctrl);
	finalize();
}

void ControlledRoute::unregister(Section * section)
{
	SectionController * controller =
		ControllerRegistry::instance().find<SectionController>(section);

	unregister(controller);
}

void ControlledRoute::unregister(SectionController * controller)
{
	controller->unlock();

	disconnectSectionController(controller);

	qDebug().noquote() << "Unregister: " << *controller;

	sections.remove(controller->section());
	controller->nextController(nullptr);

	SignalControllerProxy * signal_ctrl = getSignalController(controller->section());
	if (signal_ctrl != nullptr)
	{
		signal_ctrl->setBatch(nullptr);
	}

	while ((track.size() > 0) && (track.front()->section() == controller->section()))
	{
		RailPart    *    part      = track.front();
		Device     *     device    = dynamic_cast<Device *>(part);
		BaseController * part_ctrl =
			ControllerRegistry::instance().find<BaseController>(device);

		RegularSwitchControllerProxy   *   rs  =
			dynamic_cast<RegularSwitchControllerProxy *>(part_ctrl);
		DoubleCrossSwitchControllerProxy * dcs =
			dynamic_cast<DoubleCrossSwitchControllerProxy *>(part_ctrl);

		part->reserve(false);
		if (rs != nullptr)
		{
			rs->unlock();
			rs->setBatch(nullptr);

			disconnect(
				rs, &RegularSwitchControllerProxy::stop,
				&statechart, &RouteStatechart::failed);
		}

		if (dcs != nullptr)
		{
			dcs->unlock();
			dcs->setBatch(nullptr);

			disconnect(
				dcs, &DoubleCrossSwitchControllerProxy::stop,
				&statechart, &RouteStatechart::failed);
		}

		track.pop_front();
	}

	qDebug().noquote() << "Unregister: " << countAllocatedSections() << "sections left";
}

void ControlledRoute::disconnectSectionController(SectionController * controller)
{
	controller->setBatch(nullptr);

	disconnect(
		controller, &SectionController::entered,
		this, &ControlledRoute::entered);
	disconnect(
		controller, &SectionController::left,
		this, &ControlledRoute::left);
	disconnect(
		controller, &SectionController::tryUnblock,
		this, &ControlledRoute::tryUnblock);
	disconnect(
		controller, &SectionController::unregister,
		this, qOverload<>(&ControlledRoute::unregister));
	disconnect(
		controller, &SectionController::stop,
		&statechart, &RouteStatechart::failed);
}

void ControlledRoute::finalize()
{
	if (countAllocatedSections() == 1)
	{
		if (sections.back() == last_section)
		{
			// This marks the complete route reachead.
			qDebug().noquote() << "Finalize:   " << String::bold("Finished!");

			emit disable();
		}
		else
		{
			// This marks the next main signal reached.
			qDebug().noquote() << "Finalize:   " << String::bold("Reached!");
		}
	}
}

/*************************************************************************
**                                                                      **
**       Convenience methods                                            **
**                                                                      **
*************************************************************************/

size_t ControlledRoute::countAllocatedSections()
{
	return std::count_if(sections.begin(), sections.end(), [&](Section * section)
	{
		return !(section->isFree() || (section->state() == SectionState::PASSED));
	});
}

SectionController * ControlledRoute::getSectionController(Section * section) const
{
	return ControllerRegistry::instance().find<SectionController>(section);
}

SignalControllerProxy * ControlledRoute::getSignalController(Section * section) const
{
	const std::vector<Signal *> & section_signals = section->getSignals(direction);
	SignalControllerProxy    *    controller      = nullptr;

	if (section_signals.size() > 0)
	{
		Device * device = dynamic_cast<Device *>(section_signals.front());

		controller = ControllerRegistry::instance().find<SignalControllerProxy>(device);
	}
	return controller;
}

void ControlledRoute::collectSignalControllers(
	std::vector<SignalControllerProxy *>       &      controllers,
	std::function<bool(SignalControllerProxy * ctrl)> guard) const
{
	controllers.clear();
	for (Section * section : sections)
	{
		SignalControllerProxy * controller = getSignalController(section);

		if ((controller != nullptr) && guard(controller))
		{
			controllers.push_back(controller);
		}
	}
}

void ControlledRoute::collectSignalControllers(
	std::vector<SignalControllerProxy *> & controllers,
	const bool                             unlocked) const
{
	collectSignalControllers(controllers, [unlocked](SignalControllerProxy * proxy)
	{
		return (proxy->isUnlocked() == unlocked) && (!proxy->isPassed());
	});
}

void ControlledRoute::collectSectionControllers(std::vector<SectionController *> & controllers) const
{
	controllers.clear();
	for (Section * section : sections)
	{
		SectionController * controller =
			ControllerRegistry::instance().find<SectionController>(section);

		controllers.push_back(controller);
	}
}

bool ControlledRoute::isCurved(const RailPart * part)
{
	return part->reserved() && part->isCurved();
}

ControlledRoute::operator QListWidgetItem * ()
{
	return &list_item;
}

void ControlledRoute::dump() const
{
	__METHOD__;

	std::vector<SignalControllerProxy *> controllers;

	Route::dump();
	qDebug() << "---";
	collectSignalControllers(controllers);
	for (SignalControllerProxy * proxy : controllers)
	{
		qDebug().noquote() << "     " << *proxy;
	}
}

/*************************************************************************
**                                                                      **
**       Implementation of RouteStatemachine::OperationCallback         **
**                                                                      **
*************************************************************************/

void ControlledRoute::resetTransaction()
{
	Batch::reset();
}

void ControlledRoute::fail()
{
	__METHOD__;

	qCritical().noquote() << String::red("Failing route:") << list_item.text();
	Batch::dump();

	dump();
	BeerModeService::instance().disableBeerMode(this);
}

void ControlledRoute::tryComplete()
{
	__METHOD__;

	Batch::tryComplete();
}

bool ControlledRoute::isCompleted()
{
	return Batch::isCompleted();
}

bool ControlledRoute::isTour()
{
	return state == SectionState::TOUR;
}

/*************************************************************************
**                                                                      **
**       Propose actions to controller proxy instances                  **
**                                                                      **
*************************************************************************/

void ControlledRoute::turnSwitches()
{
	__METHOD__;

	for (RailPart * part : track)
	{
		Device     *     device     = dynamic_cast<Device *>(part);
		BaseController * controller =
			ControllerRegistry::instance().find<BaseController>(device);

		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::turn);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::turn);
	}
}

void ControlledRoute::turnFlanks()
{
	__METHOD__;

	// Now turn
	for (RegularSwitch * flank_switch : flank_switches)
	{
		RegularSwitchControllerProxy * controller =
			ControllerRegistry::instance().find<RegularSwitchControllerProxy>(flank_switch);

		controller->setBatch(this);
		controller->turn();
		qDebug().noquote() << *controller;
	}
}

void ControlledRoute::enableSignals()
{
	__METHOD__;

	collectSignalControllers(controllers_unlocked, true);
	collectSignalControllers(controllers_locked,   false);

	for (auto it = controllers_unlocked.rbegin(); it != controllers_unlocked.rend(); ++it)
	{
		SignalControllerProxy * controller = *it;

		controller->enable();
		qDebug().noquote() << *controller;
	}
}

void ControlledRoute::extendSignals()
{
	__METHOD__;

	for (auto it = controllers_locked.rbegin(); it != controllers_locked.rend(); ++it)
	{
		SignalControllerProxy * controller = *it;

		controller->extend();
		qDebug().noquote() << *controller;
	}
}

void ControlledRoute::enableSections()
{
	__METHOD__;

	for (
		auto it = sections.rbegin();
		it != sections.rend();
		++it)
	{
		SectionController * controller =
			ControllerRegistry::instance().find<SectionController>(*it);

		if (it == sections.rbegin())
		{
			controller->enable(last_section == nullptr);
		}
		else
		{
			controller->enable(true);
		}
	}
}

void ControlledRoute::disableSections()
{
	__METHOD__;

	std::vector<SectionController *> controllers;

	collectSectionControllers(controllers);
	for (SectionController * controller : controllers)
	{
		controller->disable();
	}
}

void ControlledRoute::disableSignals()
{
	__METHOD__;

	std::vector<SignalControllerProxy *> controllers;

	collectSignalControllers(controllers);
	for (SignalControllerProxy * controller : controllers)
	{
		controller->disable();
	}
}

void ControlledRoute::unlockFlanks()
{
	__METHOD__;

	for (RegularSwitch * flank_switch : flank_switches)
	{
		RegularSwitchControllerProxy * controller =
			ControllerRegistry::instance().find<RegularSwitchControllerProxy>(flank_switch);

		controller->setBatch(nullptr);
	}
}

void ControlledRoute::unlockRailParts()
{
	__METHOD__;

	for (RailPart * part : track)
	{
		Device     *     device     = dynamic_cast<Device *>(part);
		BaseController * controller =
			ControllerRegistry::instance().find<BaseController>(device);

		part->reserve(false);
		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::unlock);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::unlock);
	}
}

void ControlledRoute::unlockSections()
{
	__METHOD__;

	std::vector<SectionController *> controllers;

	collectSectionControllers(controllers);
	for (SectionController * controller : controllers)
	{
		// Unlock will cause unregister which also unlock rail parts
		controller->unlock();
	}
}
