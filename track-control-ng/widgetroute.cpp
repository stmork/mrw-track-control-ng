//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QMetaMethod>

#include <util/method.h>
#include <util/stringutil.h>
#include <statecharts/timerservice.h>
#include <ctrl/controllerregistry.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/sectioncontroller.h>
#include <ctrl/signalcontrollerproxy.h>

#include "widgetroute.h"

using namespace mrw::util;
using namespace mrw::statechart;
using namespace mrw::model;
using namespace mrw::ctrl;

using LockState = Device::LockState;
using Symbol    = Signal::Symbol;

WidgetRoute::WidgetRoute(
	const bool           dir,
	const SectionState   wanted_state,
	RailPart      *      first,
	QObject       *      parent) :
	Route(dir, wanted_state, first, parent),
	statechart(nullptr)
{
	list_item.setText(first->section()->key());
	list_item.setData(USER_ROLE, QVariant::fromValue(this));

	connect(
		&ControllerRegistry::instance(), &ControllerRegistry::completed,
		&statechart, &RouteStatechart::completed,
		Qt::QueuedConnection);
	connect(
		this, &WidgetRoute::disable,
		&statechart, &RouteStatechart::disable,
		Qt::QueuedConnection);
	connect(
		this, &WidgetRoute::turn,
		&statechart, &RouteStatechart::extended,
		Qt::QueuedConnection);
	connect(
		&statechart, &RouteStatechart::finished,
		this, &WidgetRoute::finished,
		Qt::QueuedConnection);
	connect(
		&statechart, &RouteStatechart::activated,
		this, &WidgetRoute::dump,
		Qt::QueuedConnection);

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);
	statechart.enter();
}

WidgetRoute::~WidgetRoute()
{
	statechart.disable();
	statechart.exit();
}

/*************************************************************************
**                                                                      **
**       Preparing extension of route                                   **
**                                                                      **
*************************************************************************/

void WidgetRoute::prepare(
	Section  * last_section,
	RailPart * last_part)
{
	std::vector<SectionController *> controllers;

	Route::prepare(last_section, last_part);

	collectSectionControllers(controllers);
	for (SectionController * controller : controllers)
	{
		controller->setAutoUnlock(auto_unblock);
		if (controller->section()->lock() != LockState::LOCKED)
		{
			connect(
				controller, &SectionController::left,
				this, &WidgetRoute::left,
				Qt::DirectConnection);
			connect(
				controller, &SectionController::tryUnblock,
				this, &WidgetRoute::tryUnblock,
				Qt::DirectConnection);
			connect(
				controller, &SectionController::unregister,
				this, qOverload<>(&WidgetRoute::unregister),
				Qt::DirectConnection);
			connect(
				controller, &SectionController::stop,
				&statechart, &RouteStatechart::failed,
				Qt::DirectConnection);
		}
	}

	prepareTrack(last_section, last_part);
	prepareSignals(last_section, last_part);
}

void WidgetRoute::prepareTrack(
	Section  * last_section,
	RailPart * last_part)
{
	Q_UNUSED(last_section);

	for (auto it = track.rbegin(); *it != last_part; ++it)
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
				Qt::DirectConnection);
		}

		if (dcs != nullptr)
		{
			connect(
				dcs, &DoubleCrossSwitchControllerProxy::stop,
				&statechart, &RouteStatechart::failed,
				Qt::DirectConnection);
		}
	}
}

void WidgetRoute::prepareSignals(Section * last_section, RailPart * last_part)
{
	Q_UNUSED(last_section);
	Q_UNUSED(last_part);

	SignalControllerProxy * main_controller = nullptr;
	size_t                  curved          = 0;

	for (auto it = sections.rbegin(); it != sections.rend(); ++it)
	{
		Section        *        section    = *it;
		SignalControllerProxy * controller = getSignalController(section);
		std::vector<RailPart *> rails;

		section->parts<RailPart>(rails, [](const RailPart * part)
		{
			return part->reserved() && part->isBranch();
		});
		curved += rails.size();

		if (controller != nullptr)
		{
			// OK, there is a signal...
			if (controller->hasDistant())
			{
				// OK, a distant signal has to reflect the main signals state.
				controller->setDistantSignal(main_controller);
			}

			if (controller->hasMain())
			{
				//  OK, a main signal starts a new block.
				main_controller = controller;
				controller->setCurved(curved);
				curved = 0;
			}

			controller->setState(state);
			controller->setSymbol(section != this->last ? Symbol::GO : Symbol::STOP);
		}
	}
}

/*************************************************************************
**                                                                      **
**       Releasing parts of route                                       **
**                                                                      **
*************************************************************************/

void WidgetRoute::left()
{
	SectionController   *   controller = dynamic_cast<SectionController *>(QObject::sender());
	SignalControllerProxy * sig_ctrl   = getSignalController(controller->section());

	qDebug().noquote() << "Left:       " << *controller;

	if (sig_ctrl != nullptr)
	{
		sig_ctrl->disable();
	}
}

void WidgetRoute::tryUnblock()
{
	SectionController   *   controller  = dynamic_cast<SectionController *>(QObject::sender());
	Section        *        section     = controller->section();
	SignalControllerProxy * sig_ctrl    = getSignalController(section);
	Signal         *        main_signal = sig_ctrl != nullptr ? sig_ctrl->mainSignal() : nullptr;

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

	finalize();
}

void WidgetRoute::unregister()
{
	unregister(dynamic_cast<SectionController *>(QObject::sender()));
	finalize();
}

void WidgetRoute::unregister(Section * section)
{
	SectionController * controller =
		ControllerRegistry::instance().find<SectionController>(section);

	unregister(controller);
}

void WidgetRoute::unregister(SectionController * controller)
{
	controller->disable();

	disconnect(
		controller, &SectionController::left,
		this, &WidgetRoute::left);
	disconnect(
		controller, &SectionController::tryUnblock,
		this, &WidgetRoute::tryUnblock);
	disconnect(
		controller, &SectionController::unregister,
		this, qOverload<>(&WidgetRoute::unregister));
	disconnect(
		controller, &SectionController::stop,
		&statechart, &RouteStatechart::failed);

	qDebug().noquote() << "Unregister: " << *controller;

	sections.remove(controller->section());
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
			disconnect(
				rs, &RegularSwitchControllerProxy::stop,
				&statechart, &RouteStatechart::failed);
		}

		if (dcs != nullptr)
		{
			dcs->unlock();
			disconnect(
				dcs, &DoubleCrossSwitchControllerProxy::stop,
				&statechart, &RouteStatechart::failed);
		}

		track.pop_front();
	}

	qDebug().noquote() << "Unregister: " << countAllocatedSections() << "sections left";
}

void WidgetRoute::finalize()
{
	if (countAllocatedSections() == 1)
	{
		if (sections.back() == last)
		{
			qDebug().noquote() << "Finalize:   " << String::bold("Finished!");

			emit disable();
		}
		else
		{
			qDebug().noquote() << "Finalize:   " << String::bold("Reached!");
		}
	}
}

size_t WidgetRoute::countAllocatedSections()
{
	return std::count_if(sections.begin(), sections.end(), [&](Section * section)
	{
		return !
			(section->isFree() ||
				(section->state() == SectionState::PASSED));
	});
}

void WidgetRoute::dump() const
{
	__METHOD__;

	std::vector<SignalControllerProxy *> controllers;

	Route::dump();
	qDebug() << "---";
	collectSignalController(controllers);
	for (SignalControllerProxy * proxy : controllers)
	{
		qDebug().noquote() << "     " << *proxy;
	}
}

void WidgetRoute::reset()
{
	ControllerRegistry::instance().reset();
}

void WidgetRoute::tryComplete()
{
	__METHOD__;

	ControllerRegistry::instance().complete();
}

bool WidgetRoute::isCompleted()
{
	return ControllerRegistry::instance().isCompleted();
}

WidgetRoute::operator QListWidgetItem * ()
{
	return &list_item;
}

/*************************************************************************
**                                                                      **
**       Convenience methods                                            **
**                                                                      **
*************************************************************************/

SignalControllerProxy * WidgetRoute::getSignalController(Section * section) const
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

void WidgetRoute::collectSignalController(std::vector<SignalControllerProxy *> & controllers) const
{
	controllers.clear();
	for (Section * section : sections)
	{
		SignalControllerProxy * controller = getSignalController(section);

		if (controller != nullptr)
		{
			controllers.push_back(controller);
		}
	}
}

void WidgetRoute::collectSectionControllers(std::vector<SectionController *> & controllers) const
{
	controllers.clear();
	for (Section * section : sections)
	{
		SectionController * controller =
			ControllerRegistry::instance().find<SectionController>(section);

		controllers.push_back(controller);
	}
}

/*************************************************************************
**                                                                      **
**       Propose actions to controller proxy instances                  **
**                                                                      **
*************************************************************************/

void WidgetRoute::turnSwitches()
{
	__METHOD__;

	size_t count = 0;

	for (RailPart * part : track)
	{
		Device     *     device     = dynamic_cast<Device *>(part);
		BaseController * controller =
			ControllerRegistry::instance().find<BaseController>(device);

		if ((device != nullptr) && (device->lock() != LockState::LOCKED))
		{
			count++;
		}

		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::turn);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::turn);
	}

	if (count == 0)
	{
		ControllerRegistry::instance().complete();
	}
}

void WidgetRoute::unlockSwitches()
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

void WidgetRoute::activateSections()
{
	__METHOD__;

	size_t count   = 0;

	for (
		auto it = sections.rbegin();
		it != sections.rend();
		++it)
	{
		SectionController * controller =
			ControllerRegistry::instance().find<SectionController>(*it);

		if (it == sections.rbegin())
		{
			controller->enable(last == nullptr);
		}
		else
		{
			controller->enable(true);
		}
		count++;
	}

	if (count == 0)
	{
		ControllerRegistry::instance().complete();
	}
}

void WidgetRoute::deactivateSections()
{
	__METHOD__;

	std::vector<SectionController *> controllers;

	collectSectionControllers(controllers);
	for (SectionController * controller : controllers)
	{
		controller->disable();
	}
}

void WidgetRoute::turnSignals()
{
	__METHOD__;

	std::vector<SignalControllerProxy *> controllers;
	size_t                               count = 0;

	collectSignalController(controllers);
	for (auto it = controllers.rbegin(); it != controllers.rend(); ++it)
	{
		SignalControllerProxy * controller = *it;

		// TODO: How to handle locking? We have to lock main signals but distant signals are locked elsewehere.
//		if (controller->isUnlocked())
		{
			qDebug().noquote() << *controller;
			controller->enable();
			count++;
		}
	}

	if (count == 0)
	{
		ControllerRegistry::instance().complete();
	}
}

void WidgetRoute::unlockSignals()
{
	__METHOD__;

	std::vector<SignalControllerProxy *> controllers;

	collectSignalController(controllers);
	for (SignalControllerProxy * controller : controllers)
	{
		controller->disable();
	}
}
