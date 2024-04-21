//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>

#include <can/cansettings.h>
#include <util/duration.h>
#include <util/termhandler.h>

#include "updateservice.h"

using namespace mrw::util;
using namespace mrw::can;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);

	Duration::pattern();

	TermHandler           term_handler;
	CanSettings           settings;
	UpdateService         service(
		argc > 1 ? argv[1] : "/lib/firmware/mrw/mrw-firmware-m32.hex",
		settings.interface(), settings.plugin());

	return app.exec();
}
