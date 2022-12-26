//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/method.h>
#include <util/dumphandler.h>
#include <util/termhandler.h>

#include "trackerservice.h"

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication      app(argc, argv);

	Method::pattern();

	TermHandler           term_handler( { SIGTERM, SIGINT } );
	ModelRepository       repo(ModelRepository::proposeModelName());
	TrackerService        tracker(repo);
	DumpHandler           dumper([&]()
	{
		tracker.info();
	});

	return app.exec();
}
