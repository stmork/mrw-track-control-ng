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
	QString selected = select(interface, plugin);

	device = can_bus->createDevice(plugin, selected, &error);
	if (device != nullptr)
	{
		connect(device, &QCanBusDevice::framesReceived, this, &MrwBusService::receive);

#ifdef MRW_VERBOSE
		connect(device, &QCanBusDevice::stateChanged, [] (auto state)
		{
			qDebug() << state;
		});
		connect(device, &QCanBusDevice::errorOccurred, [] (auto reason)
		{
			qCritical() << reason;
		});
#endif

		device->connectDevice();
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
		delete device;
	}
}

bool MrwBusService::valid()
{
	return
		(device != nullptr) &&
		(device->state() == QCanBusDevice::ConnectedState);
}

bool MrwBusService::list()
{
	bool success = true;

	for (const QString & plugin : can_bus->plugins())
	{
		QString error;

		qInfo().noquote() << plugin;
		const QList<QCanBusDeviceInfo> & infos = can_bus->availableDevices(plugin, &error);

		if (!error.isEmpty())
		{
			qCritical().noquote() << error;
			success = false;
		}
		else
		{
			for (const QCanBusDeviceInfo & info : infos)
			{

				qInfo().noquote().nospace() << "  " << info.name() << ": " << info.description();
			}
		}
	}
	return success;
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
	QString error;
	const QList<QCanBusDeviceInfo> & infos = can_bus->availableDevices(plugin, &error);

	if (error.isEmpty())
	{
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
