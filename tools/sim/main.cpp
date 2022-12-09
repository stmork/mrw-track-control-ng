//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/termhandler.h>

#include "simulatorservice.h"

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);
	TermHandler           term_handler( { SIGTERM, SIGINT } );
	QString               interface = "can0";
	QString               plugin    = "virtualcan";
	SimulatorService      simulator(argc > 1 ? argv[1] : "", interface.toLatin1().data(), plugin.toLatin1().data());

	return app.exec();
}
