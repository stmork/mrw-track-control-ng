//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/dumphandler.h>
#include <util/termhandler.h>

#include "simulatorservice.h"

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);
	TermHandler           term_handler( { SIGTERM, SIGINT } );
	ModelRepository       repo(ModelRepository::proposeModelName());
	SimulatorService      simulator(repo);
	DumpHandler           dumper([&]()
	{
		simulator.info();
	});

	return app.exec();
}
