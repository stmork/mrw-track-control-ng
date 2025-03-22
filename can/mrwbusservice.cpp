//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <unistd.h>

#include <QCoreApplication>
#include <QCanBus>
#include <QCanBusDeviceInfo>
#include <QDebug>
#include <QTimer>

#include <util/method.h>
#include <can/mrwbusservice.h>

using namespace std::chrono_literals;
using namespace mrw::can;

const std::chrono::microseconds MrwBusService::retry = 20ms;

MrwBusService::MrwBusService(
	const QString & interface,
	const QString & plugin,
	QObject    *    parent,
	const bool      auto_connect) :
	QObject(parent),
	can_bus(QCanBus::instance())

{
	QString error;
	const QString selected = select(interface, plugin);

	can_device = can_bus->createDevice(plugin, selected, &error);
	if (can_device != nullptr)
	{
		can_device->setConfigurationParameter(QCanBusDevice::BitRateKey, QVariant());
		connect(
			can_device, &QCanBusDevice::framesReceived,
			this, &MrwBusService::receive,
			Qt::QueuedConnection);
		connect(
			can_device, &QCanBusDevice::stateChanged,
			this, &MrwBusService::stateChanged,
			Qt::QueuedConnection);

		connect(
			can_device, &QCanBusDevice::errorOccurred, [this] (auto reason)
		{
			qCritical().noquote() << "CAN bus error occured:" << reason;
			qCritical().noquote() << "CAN bus status:       " << can_device->busStatus();
		});

		if (auto_connect)
		{
			if (!can_device->connectDevice())
			{
				qCritical().noquote() << "Cannot connect to CAN device!";
			}
		}
	}
	else
	{
		qCritical().noquote() << "Cannot create CAN device:" << error;
	}
}

MrwBusService::~MrwBusService()
{
	qInfo(" Shutting down MRW bus service.");
	if (can_device != nullptr)
	{
		can_device->disconnectDevice();
		delete can_device;
	}
}

bool MrwBusService::valid() noexcept
{
	return
		(can_device != nullptr) &&
		(can_device->state() == QCanBusDevice::ConnectedState);
}

bool MrwBusService::list() noexcept
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

bool MrwBusService::write(const MrwMessage & message) noexcept
{
	qDebug().noquote() << message;

	if (can_device != nullptr)
	{
		if (can_device->framesAvailable() > 0)
		{
			QCoreApplication::processEvents();
		}

		if (can_device->writeFrame(message))
		{
			can_device->waitForFramesWritten(10);
			return true;
		}
		else if (can_device->busStatus() == QCanBusDevice::CanBusStatus::Good)
		{
			usleep(retry.count());
			qWarning("Retrying...");
			return can_device->writeFrame(message);
		}
	}
	return false;
}

void MrwBusService::process(const MrwMessage & message)
{
	qDebug().noquote() << message;
}

QString MrwBusService::select(
	const QString & interface,
	const QString & plugin) noexcept
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
	else
	{
		qCritical().noquote() << "Cannot list available CAN devices: " << error;
	}

	return "";
}

void MrwBusService::stateChanged(QCanBusDevice::CanBusDeviceState state) noexcept
{
	switch (state)
	{
	case QCanBusDevice::ConnectedState:
		qInfo("CAN bus connected.");
		emit connected();
		break;

	case QCanBusDevice::UnconnectedState:
		qInfo("CAN bus disconnected.");
		emit disconnected();
		break;

	default:
		// States intentionally ignored.
		qDebug().noquote() << "CAN bus state change: " << state;
		break;
	}
}

void MrwBusService::receive() noexcept
{
	for (const QCanBusFrame & frame : can_device->readAllFrames())
	{
		process(MrwMessage(frame));
	}
}
