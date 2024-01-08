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

	qInfo("  Shutting down MRW message dispatcher.");
}

void MrwMessageDispatcher::emergencyStop()
{
	std::vector<Section *> sections;

	model->parts<Section>(sections);
	for (Section * section : sections)
	{
		const MrwMessage message = section->command(SETROF);

		write(message);
	}
}

void MrwMessageDispatcher::process(const MrwMessage & message)
{
	const ControllerId dst = message.sid();

	if (message.isResponse())
	{
		// A CAN controller sent a response...
		if (dst == CAN_GATEWAY_ID)
		{
			// ...and we are addressed.
			const ControllerId     id         = message.eid();
			const UnitNo           unit_no    = message.unitNo();
			Device        *        device     = model->deviceById(id, unit_no);
			ControllerRegistrand * controller =
				ControllerRegistry::instance().find<ControllerRegistrand>(device);

			if (controller != nullptr)
			{
				// OK, we found a device unit controller which can process.
				if (controller->process(message))
				{
					// OK, the message was processed.
					return;
				}
			}
			else
			{
				// OK, what to do if no controller was found. Mostly as a
				// non device unit response such as PING.
				if (filter(message))
				{
					// OK, the message was filtered.
					return;
				}
			}
		}
	}
	else
	{
		if (dst == CAN_BROADCAST_ID)
		{
			// OK, we received a broadcast request.
			if ((message.command() == SENSOR) && (message[0] == SENSOR_LIGHT))
			{
				emit brightness(message[1]);
				return;
			}
		}
	}

	qDebug().noquote() << message << "---";
}

bool MrwMessageDispatcher::filter(const MrwMessage & message)
{
	Controller * controller = model->controllerById(message.eid());

	if (controller != nullptr)
	{
		// OK, we found the sending CAN controller.
		switch (message.command())
		{
		case PING:
			return false;

		case GETVER:
			if (message.size() == 4)
			{
				const unsigned major = message[1];
				const unsigned minor = message[2] | (message[3] << 8);

				qInfo("Controller: %03u V%u.%u", controller->id(), major, minor);
			}
			return true;

		case SENSOR:
			return true;

		default:
			return false;
		}
	}
	return false;
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
