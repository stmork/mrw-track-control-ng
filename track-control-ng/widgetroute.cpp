//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QMetaMethod>

#include <statecharts/timerservice.h>
#include <ctrl/controllerregistry.h>
#include <ctrl/regularswitchcontrollerproxy.h>
#include <ctrl/doublecrossswitchcontrollerproxy.h>
#include <ctrl/sectioncontroller.h>

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
	statechart.exit();
}

void WidgetRoute::turn()
{
	QMetaObject::invokeMethod(&statechart, &RouteStatechart::extended, Qt::QueuedConnection);
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
	for (RailPart * part : track)
	{
		Device * device = dynamic_cast<Device *>(part);

		if (device != nullptr)
		{
			ControllerRegistrand * controller = ControllerRegistry::instance().find(device);

			RegularSwitchControllerProxy * rs_ctrl =
				dynamic_cast<RegularSwitchControllerProxy *>(controller);
			DoubleCrossSwitchControllerProxy * dcs_ctrl =
				dynamic_cast<DoubleCrossSwitchControllerProxy *>(controller);

			if (rs_ctrl != nullptr)
			{
				QMetaObject::invokeMethod(rs_ctrl, &RegularSwitchControllerProxy::turn, Qt::QueuedConnection);
			}
			if (dcs_ctrl != nullptr)
			{
				QMetaObject::invokeMethod(dcs_ctrl, &DoubleCrossSwitchControllerProxy::turn, Qt::QueuedConnection);
			}
		}
	}
}

void WidgetRoute::activateSections()
{
	for (Section * section : sections)
	{
		ControllerRegistrand * controller = ControllerRegistry::instance().find(section);

		SectionController * proxy = dynamic_cast<SectionController *>(controller);

		proxy->enable();
	}
}

void WidgetRoute::turnSignals()
{
	// TODO: Implement signal state machine first.
	statechart.turned();
}

void WidgetRoute::deactivateSections()
{
	for (Section * section : sections)
	{
		ControllerRegistrand * controller = ControllerRegistry::instance().find(section);

		SectionController * proxy = dynamic_cast<SectionController *>(controller);

		proxy->disable();
	}
}
