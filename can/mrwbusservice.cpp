//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <chrono>

#include <QCoreApplication>
#include <QCanBus>
#include <QCanBusDeviceInfo>
#include <QThread>
#include <QTimer>
#include <QVariant>

#include <util/method.h>
#include <can/mrwbusservice.h>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace mrw::can;

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
			qCCritical(log).noquote() << "CAN bus error occured:" << reason;
			qCCritical(log).noquote() << "CAN bus status:       " << can_device->busStatus();
		});

		if (auto_connect)
		{
			if (!can_device->connectDevice())
			{
				qCCritical(log).noquote() << "Cannot connect to CAN device!";
			}
		}
	}
	else
	{
		qCCritical(log).noquote() << "Cannot create CAN device:" << error;
	}
}

MrwBusService::~MrwBusService()
{
	qCInfo(log, " Shutting down MRW bus service.");
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

		qCInfo(log).noquote() << plugin;
		const QList<QCanBusDeviceInfo> & infos = can_bus->availableDevices(plugin, &error);

		if (!error.isEmpty())
		{
			qCCritical(log).noquote() << error;
			success = false;
		}
		else
		{
			for (const QCanBusDeviceInfo & info : infos)
			{

				qCInfo(log).noquote().nospace() << "  " << info.name() << ": " << info.description();
			}
		}
	}
	return success;
}

bool MrwBusService::write(const MrwMessage & message) noexcept
{
	qCDebug(log).noquote() << message;

	if (can_device != nullptr)
	{
		if ((can_device->framesAvailable() > 0) ||
			(can_device->framesToWrite() > 0))
		{
			qCDebug(log, "Process events...");

			QCoreApplication::processEvents();
		}

		if (can_device->writeFrame(message))
		{
			return true;
		}
		else if (can_device->busStatus() == QCanBusDevice::CanBusStatus::Good)
		{
			static constexpr microseconds retry = 20ms;

			qCWarning(log, "Retrying...");

			QThread::usleep(retry.count());
			return can_device->writeFrame(message);
		}
	}
	return false;
}

void MrwBusService::process(const MrwMessage & message)
{
	qCDebug(log).noquote() << message;
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
		qCCritical(log).noquote() << "Cannot list available CAN devices: " << error;
	}

	return "";
}

void MrwBusService::stateChanged(QCanBusDevice::CanBusDeviceState state) noexcept
{
	switch (state)
	{
	case QCanBusDevice::ConnectedState:
		qCInfo(log, "CAN bus connected.");
		emit connected();
		break;

	case QCanBusDevice::UnconnectedState:
		qCInfo(log, "CAN bus disconnected.");
		emit disconnected();
		break;

	default:
		// States intentionally ignored.
		qCDebug(log).noquote() << "CAN bus state change: " << state;
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
