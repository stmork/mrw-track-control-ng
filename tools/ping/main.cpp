//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>
#include <QCanBus>
#include <QDebug>

#include <can/mrwmessage.h>

using namespace mrw::can;

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);
	QCanBus     *    can_bus = QCanBus::instance();
	QString          error;

	for (const QString & plugin : can_bus->plugins())
	{
		qInfo().noquote() << plugin;
		for (const QCanBusDeviceInfo & device : can_bus->availableDevices(plugin, &error))
		{
			qInfo().noquote().nospace() << "  " << device.name() << ": " << device.description();
		}
	}

	QCanBusDevice * device = can_bus->createDevice("socketcan", "can0", &error);

	QObject::connect(device, &QCanBusDevice::framesReceived, [device]()
	{
		for (const QCanBusFrame & frame : device->readAllFrames())
		{
			qInfo() << MrwMessage(frame);
		}
	});

	device->connectDevice();

	MrwMessage message(PING);

	device->writeFrame(message);
	qInfo() << message;

	return app.exec();
}
