//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QMetaMethod>

#include <util/method.h>
#include <statecharts/timerservice.h>
#include <ctrl/controllerregistry.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/sectioncontroller.h>
#include <ctrl/signalcontrollerproxy.h>

#include "widgetroute.h"

using namespace mrw::statechart;
using namespace mrw::model;
using namespace mrw::ctrl;

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

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);
	statechart.enter();
}

WidgetRoute::~WidgetRoute()
{
	statechart.disable();
	statechart.exit();
}

void WidgetRoute::prepare()
{
	std::vector<SectionController *> controllers;

	collectSectionControllers(controllers);
	for (SectionController * controller : controllers)
	{
		disconnect(
			controller, &SectionController::left,
			this, &WidgetRoute::left);
	}

	Route::prepare();

	collectSectionControllers(controllers);
	for (SectionController * controller : controllers)
	{
		connect(
			controller, &SectionController::left,
			this, &WidgetRoute::left,
			Qt::QueuedConnection);
	}
}

void WidgetRoute::left()
{
	SectionController * controller = dynamic_cast<SectionController *>(QObject::sender());

	sections.remove(controller->section());

	qDebug().noquote() << "Left:" << *controller;
	qDebug().noquote() << "Left:" << sections.size();

	if ((sections.size() == 1) && (sections.back() == last))
	{
		qDebug().noquote() << "Left: FINITO!";
		emit finished();
	}
}

void WidgetRoute::collectSignals()
{
	main_signals.clear();
	for (Section * section : sections)
	{
		section->parts<Signal>(main_signals, [&](const Signal * signal)
		{
			return
				(signal->direction() == direction) &&
				((signal->type() & Signal::MAIN_SIGNAL) != 0);
		});
	}
}

void WidgetRoute::collectSectionControllers(std::vector<SectionController *> & controllers)
{
	controllers.clear();
	for (Section * section : sections)
	{
		SectionController * controller =
			ControllerRegistry::instance().find<SectionController>(section);

		if (controller != nullptr)
		{
			controllers.push_back(controller);
		}
	}
}

void WidgetRoute::reset()
{
	ControllerRegistry::instance().reset();
}

WidgetRoute::operator QListWidgetItem * ()
{
	return &list_item;
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

		BaseController::callback<RegularSwitchControllerProxy>(
			controller,     &RegularSwitchControllerProxy::turn);
		BaseController::callback<DoubleCrossSwitchControllerProxy>(
			controller, &DoubleCrossSwitchControllerProxy::turn);

		if (controller != nullptr)
		{
			count++;
		}
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

		controller->enable();
		count++;
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
