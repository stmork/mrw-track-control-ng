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
		&statechart, &RouteStatechart::turned,
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

void WidgetRoute::turn()
{
	QMetaObject::invokeMethod(&statechart, &RouteStatechart::extended, Qt::QueuedConnection);
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

	qDebug().noquote() << "Left:" << *controller;
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
		Device * device = dynamic_cast<Device *>(part);

		RegularSwitchControllerProxy   *   rs_ctrl  =
			ControllerRegistry::instance().find<RegularSwitchControllerProxy>(device);
		DoubleCrossSwitchControllerProxy * dcs_ctrl =
			ControllerRegistry::instance().find<DoubleCrossSwitchControllerProxy>(device);

		if (rs_ctrl != nullptr)
		{
			rs_ctrl->turn();
			count++;
		}
		if (dcs_ctrl != nullptr)
		{
			dcs_ctrl->turn();
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
		Device * device = dynamic_cast<Device *>(part);

		RegularSwitchControllerProxy   *   rs_ctrl  =
			ControllerRegistry::instance().find<RegularSwitchControllerProxy>(device);
		DoubleCrossSwitchControllerProxy * dcs_ctrl =
			ControllerRegistry::instance().find<DoubleCrossSwitchControllerProxy>(device);

		if (rs_ctrl != nullptr)
		{
			rs_ctrl->unlock();
		}
		if (dcs_ctrl != nullptr)
		{
			dcs_ctrl->unlock();
		}
	}
}

void WidgetRoute::activateSections()
{
	__METHOD__;

	size_t count = 0;

	for (Section * section : sections)
	{
		SectionController * controller = ControllerRegistry::instance().find<SectionController>(section);

		controller->enable();
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
		SectionController * controller = ControllerRegistry::instance().find<SectionController>(section);

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
		SignalControllerProxy * controller = ControllerRegistry::instance().find<SignalControllerProxy>(signal->device());

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
		SignalControllerProxy * controller = ControllerRegistry::instance().find<SignalControllerProxy>(signal->device());

		controller->disable();
	}
}
