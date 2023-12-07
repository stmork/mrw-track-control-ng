//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>

#include <can/cansettings.h>
#include <util/method.h>
#include <util/termhandler.h>

#include "updateservice.h"

using namespace mrw::util;
using namespace mrw::can;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);

	Method::pattern();

	TermHandler           term_handler;
	CanSettings           settings;
	UpdateService         service(settings.interface(), settings.plugin());

	return app.exec();
}
