//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>
#include <QCanBus>
#include <QTimer>
#include <QDebug>

#include <unistd.h>

#include <can/cansettings.h>
#include <can/mrwbusservice.h>
#include <util/method.h>
#include <util/termhandler.h>

using namespace mrw::can;
using namespace mrw::util;

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	Method::pattern();

	TermHandler      term_handler;
	CanSettings      settings;
	MrwBusService    service(settings.interface(), settings.plugin());

	service.list();

	QObject::connect(&service, &MrwBusService::connected, [&]()
	{
		const MrwMessage message(PING);

		service.write(message);
	});

	return app.exec();
}
