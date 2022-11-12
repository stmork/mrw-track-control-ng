//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>
#include <QCanBus>
#include <QDebug>

#include <can/mrwmessage.h>
#include <can/mrwbusservice.h>

using namespace mrw::can;

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);
	MrwMessage       message(PING);
	MrwBusService    service("can0");

	service.list();

	service.write(message);

	return app.exec();
}
