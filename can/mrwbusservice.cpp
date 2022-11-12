//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCanBus>
#include <QCanBusDeviceInfo>
#include <QDebug>

#include "mrwbusservice.h"

using namespace mrw::can;

MrwBusService::MrwBusService(
	const char * interface,
	const char * plugin,
	QObject   *  parent) : QObject(parent),
	can_bus(QCanBus::instance())

{
	QString error;

	device = can_bus->createDevice(plugin, select(interface, plugin), &error);
	if (device != nullptr)
	{
		device->connectDevice();
		connect(device, &QCanBusDevice::framesReceived, this, &MrwBusService::receive);
	}
	else
	{
		qCritical().noquote() << error;
	}
}

MrwBusService::~MrwBusService()
{
	if (device != nullptr)
	{
		device->disconnectDevice();
	}
}

bool MrwBusService::valid()
{
	return
		(device != nullptr) &&
		(device->state() == QCanBusDevice::ConnectedState);
}

void MrwBusService::list()
{
	QString error;

	for (const QString & plugin : can_bus->plugins())
	{
		qInfo().noquote() << plugin;
		for (const QCanBusDeviceInfo & info : can_bus->availableDevices(plugin, &error))
		{
			qInfo().noquote().nospace() << "  " << info.name() << ": " << info.description();
		}
	}
}

bool MrwBusService::write(const MrwMessage & message)
{
	qDebug() << message;
	return (device != nullptr) && (device->writeFrame(message));
}

void MrwBusService::process(const MrwMessage & message)
{
	qDebug() << message;
}

QString MrwBusService::select(
	const QString & interface,
	const QString & plugin)
{
	const QList<QCanBusDeviceInfo> & infos = can_bus->availableDevices(plugin);

	for (const QCanBusDeviceInfo & info : infos)
	{
		const QString & name = info.name();

		if (name == interface)
		{
			return interface;
		}
	}

	if (!infos.isEmpty())
	{
		return infos.first().name();
	}

	return "";
}

void MrwBusService::receive()
{
	for (const QCanBusFrame & frame : device->readAllFrames())
	{
		process(MrwMessage(frame));
	}
}
