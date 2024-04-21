//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/duration.h>
#include <util/dumphandler.h>
#include <util/termhandler.h>

#include "simulatorservice.h"

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);

	Duration::pattern();

	TermHandler           term_handler;
	ModelRepository       repo(ModelRepository::proposeModelName());
	SimulatorService      simulator(repo);
	DumpHandler           dumper([&]()
	{
		simulator.info();
	});

	return app.exec();
}
