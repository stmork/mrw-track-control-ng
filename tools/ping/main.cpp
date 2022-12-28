//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>
#include <QCanBus>
#include <QTimer>
#include <QDebug>

#include <unistd.h>

#include <can/mrwmessage.h>
#include <can/mrwbusservice.h>
#include <util/termhandler.h>

using namespace mrw::can;
using namespace mrw::util;

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);
	MrwMessage       message(PING);
	MrwBusService    service("can0", "virtualcan");
	TermHandler      term_handler( { SIGTERM, SIGINT } );

	service.list();

	QObject::connect(&service, &MrwBusService::connected, [&]()
	{
		service.write(message);
	});

	return app.exec();
}
