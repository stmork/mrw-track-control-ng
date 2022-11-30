//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include <ctrl/controllerregistry.h>
#include <statecharts/timerservice.h>
#include <util/method.h>

#include "mrwmessagedispatcher.h"

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::statechart;

MrwMessageDispatcher::MrwMessageDispatcher(
	mrw::model::ModelRailway  *  model_railway,
	const QString        &       interface,
	const QString        &       plugin,
	QObject           *          parent) :
	MrwBusService(interface, plugin, parent),
	statechart(nullptr),
	model(model_railway)
{
	__METHOD__;

	ControllerRegistry::instance().registerService(this);

	connect(
		this, &MrwBusService::connected,
		&statechart, &OperatingMode::connected);
	connect(
		&statechart, &OperatingMode::inquire,
		&ControllerRegistry::instance(), &ControllerRegistry::inquire);

	statechart.setTimerService(&TimerService::instance());
	statechart.enter();

	if (can_device->state() == QCanBusDevice::ConnectedState)
	{
		// TODO: Add API to start manually.
//		statechart.connected();
	}
}

MrwMessageDispatcher::~MrwMessageDispatcher()
{
	statechart.exit();
}

void MrwMessageDispatcher::process(const MrwMessage & message)
{
	const ControllerId dst     = message.sid();

	if (message.isResponse() && (dst == CAN_GATEWAY_ID))
	{
		const UnitNo       unit_no = message.unitNo();

		Device * device = model->deviceByUnitNo(unit_no);

		if (device != nullptr)
		{
			ControllerRegistrand * controller = ControllerRegistry::instance().find(device);

			if (controller != nullptr)
			{
				if (controller->process(message))
				{
					return;
				}
			}
		}
	}

	qDebug().noquote() << message << "---";
}
