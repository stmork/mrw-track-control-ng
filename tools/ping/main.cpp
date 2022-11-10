//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>
#include <QCanBus>
#include <QDebug>

int main(int argc, char * argv[])
{
	QCoreApplication a(argc, argv);
	QCanBus * can_bus = QCanBus::instance();

	for (const QString & plugin : can_bus->plugins())
	{
		QString error;

		qInfo().noquote() << plugin;
		for (const QCanBusDeviceInfo & device : can_bus->availableDevices(plugin, &error))
		{
			qInfo().noquote().nospace() << "  " << device.name() << ": " << device.description();
		}
	}

	return a.exec();
}
