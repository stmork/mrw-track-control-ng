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
#include <util/duration.h>
#include <util/termhandler.h>

#include "resetservice.h"

using namespace mrw::util;
using namespace mrw::can;

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	Duration::pattern();

	TermHandler      term_handler;
	CanSettings      settings;
	ResetService     service(settings.interface(), settings.plugin());

	QTimer::singleShot(5000, [&]()
	{
		app.exit(EXIT_FAILURE);
	});

	return app.exec();
}
