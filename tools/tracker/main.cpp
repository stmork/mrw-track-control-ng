//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/dumphandler.h>
#include <util/termhandler.h>

#include "trackerservice.h"

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);
	TermHandler           term_handler( { SIGTERM, SIGINT } );
	Settings              settings("model");
	SettingsGroup         group(&settings, "model");
	QString               modelname = argc > 1 ?
		argv[1] :
		settings.value("filename", "RailwayModel").toString();
	ModelRepository       repo(modelname);
	TrackerService        tracker(repo);
	DumpHandler           dumper([&]()
	{
		tracker.info();
	});

	return app.exec();
}
