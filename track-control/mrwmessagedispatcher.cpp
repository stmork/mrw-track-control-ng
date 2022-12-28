//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QDebug>

#include <util/method.h>
#include <ctrl/controllerregistry.h>

#include "mrwmessagedispatcher.h"

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

MrwMessageDispatcher::MrwMessageDispatcher(
	ModelRailway   *  model_railway,
	const QString  &  interface,
	const QString  &  plugin,
	QObject     *     parent) :
	MrwBusService(interface, plugin, parent, false),
	model(model_railway)
{
	__METHOD__;

	ControllerRegistry::instance().registerService(this);
}

MrwMessageDispatcher::~MrwMessageDispatcher()
{
	__METHOD__;

	qInfo(" Shutting down MRW message dispatcher.");
}

void MrwMessageDispatcher::process(const MrwMessage & message)
{
	const ControllerId dst     = message.sid();

	if (message.isResponse() && (dst == CAN_GATEWAY_ID))
	{
		const ControllerId     id         = message.eid();
		const UnitNo           unit_no    = message.unitNo();
		Device        *        device     = model->deviceById(id, unit_no);
		ControllerRegistrand * controller =
			ControllerRegistry::instance().find<ControllerRegistrand>(device);

		if (controller != nullptr)
		{
			if (controller->process(message))
			{
				return;
			}
		}
	}

	qDebug().noquote() << message << "---";
}

void MrwMessageDispatcher::connectBus()
{
	if (can_device->state() != QCanBusDevice::ConnectedState)
	{
		can_device->connectDevice();
	}
	else
	{
		qWarning("CAN bus already connected.");
	}
}

bool MrwMessageDispatcher::isConnected()
{
	return can_device->state() == QCanBusDevice::ConnectedState;
}
