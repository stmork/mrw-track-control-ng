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
		}
	}
}

void WidgetRoute::left()
{
	SectionController * controller = dynamic_cast<SectionController *>(QObject::sender());

	qDebug().noquote() << "Left:       " << *controller;

	main_signals.clear();
	collectSignals(controller->section());
	for (Signal * signal : main_signals)
	{
		SignalControllerProxy * sig_ctrl =
			ControllerRegistry::instance().find<SignalControllerProxy>(signal->device());

		sig_ctrl->disable();
	}
}

void WidgetRoute::tryUnblock()
{
	SectionController * controller = dynamic_cast<SectionController *>(QObject::sender());
	Section      *      section    = controller->section();

	main_signals.clear();
	collectSignals(section);
	Q_ASSERT(main_signals.size() <= 1);

	if (main_signals.size() > 0)
	{
		Section * first;

		qDebug().noquote() << "Try unblock: until >>>" << *main_signals.front();
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

	controller->disable();
	unregister(controller);
}

void WidgetRoute::unregister(SectionController * controller)
{
	disconnect(
		controller, &SectionController::left,
		this, &WidgetRoute::left);
	disconnect(
		controller, &SectionController::tryUnblock,
		this, &WidgetRoute::tryUnblock);
	disconnect(
		controller, &SectionController::unregister,
		this, qOverload<>(&WidgetRoute::unregister));

	qDebug().noquote() << "Unregister: " << *controller;

	sections.remove(controller->section());
	while (track.front()->section() == controller->section())
	{
		RailPart    *    part      = track.front();
		Device     *     device    = dynamic_cast<Device *>(part);
		BaseController * part_ctrl =
			ControllerRegistry::instance().find<BaseController>(device);

		BaseController::callback<RegularSwitchControllerProxy>(
			part_ctrl,     &RegularSwitchControllerProxy::unlock);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			part_ctrl, &DoubleCrossSwitchControllerProxy::unlock);

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
			emit finished();
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

void WidgetRoute::reset()
{
	ControllerRegistry::instance().reset();
}

WidgetRoute::operator QListWidgetItem * ()
{
	return &list_item;
}

void WidgetRoute::collectSignals()
{
	main_signals.clear();
	for (Section * section : sections)
	{
		collectSignals(section);
	}
}

void WidgetRoute::collectSignals(Section * section)
{
	section->parts<Signal>(main_signals, [&](const Signal * signal)
	{
		return
			(signal->direction() == direction) &&
			((signal->type() & Signal::MAIN_SIGNAL) != 0);
	});
}

void WidgetRoute::collectSectionControllers(std::vector<SectionController *> & controllers)
{
	controllers.clear();
	for (Section * section : sections)
	{
		SectionController * controller =
			ControllerRegistry::instance().find<SectionController>(section);

		controllers.push_back(controller);
	}
}

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
	bool   last_on = isLastSectionEnded();

	for (
		SectionTrack::reverse_iterator it = sections.rbegin();
		it != sections.rend();
		++it)
	{
		SectionController * controller =
			ControllerRegistry::instance().find<SectionController>(*it);

		if (it == sections.rbegin())
		{
			controller->enable(last_on);
			last = last_on ? nullptr : controller->section();
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

	for (Section * section : sections)
	{
		SectionController * controller =
			ControllerRegistry::instance().find<SectionController>(section);

		controller->disable();
	}
}

void WidgetRoute::turnSignals()
{
	__METHOD__;

	size_t count = 0;

	collectSignals();
	for (Signal * signal : main_signals)
	{
		SignalControllerProxy * controller =
			ControllerRegistry::instance().find<SignalControllerProxy>(signal->device());

		if (controller->isUnlocked())
		{
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

	collectSignals();
	for (Signal * signal : main_signals)
	{
		SignalControllerProxy * controller =
			ControllerRegistry::instance().find<SignalControllerProxy>(signal->device());

		controller->disable();
	}
}
