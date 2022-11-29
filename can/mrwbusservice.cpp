//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCanBus>
#include <QCanBusDeviceInfo>
#include <QDebug>
#include <QTimer>

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

	can_device = can_bus->createDevice(plugin, selected, &error);
	if (can_device != nullptr)
	{
		connect(can_device, &QCanBusDevice::framesReceived, this, &MrwBusService::receive);
		connect(can_device, &QCanBusDevice::stateChanged,   this, &MrwBusService::stateChanged);

#ifdef MRW_VERBOSE
		connect(device, &QCanBusDevice::errorOccurred, [] (auto reason)
		{
			qCritical() << reason;
		});
#endif

		can_device->connectDevice();
	}
	else
	{
		qCritical().noquote() << error;
	}
}

MrwBusService::~MrwBusService()
{
	if (can_device != nullptr)
	{
		can_device->disconnectDevice();
		delete can_device;
	}
}

bool MrwBusService::valid()
{
	return
		(can_device != nullptr) &&
		(can_device->state() == QCanBusDevice::ConnectedState);
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

#if 0
	QTimer::singleShot(10, [&] ()
	{
		can_device->writeFrame(message);
	} );

	return can_device != nullptr;
#else
	return (can_device != nullptr) && (can_device->writeFrame(message));
#endif
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

void MrwBusService::stateChanged(QCanBusDevice::CanBusDeviceState state)
{
	switch (state)
	{
	case QCanBusDevice::ConnectedState:
		emit connected();
		break;

	case QCanBusDevice::UnconnectedState:
		emit disconnected();
		break;

	default:
		// States intenionally ignored.
		break;
	}
}

void MrwBusService::receive()
{
	for (const QCanBusFrame & frame : can_device->readAllFrames())
	{
		process(MrwMessage(frame));
	}
}
