//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include <util/method.h>
#include <util/settings.h>
#include <util/dumphandler.h>
#include <util/termhandler.h>
#include <model/modelrepository.h>

#include "mainwindow.h"
#include "mrwmessagedispatcher.h"

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QApplication             app(argc, argv);

	Method::pattern();

	TermHandler              terminator( { SIGTERM, SIGINT } );
	ModelRepository          repo(ModelRepository::proposeModelName(), true);

	if (repo)
	{
		MrwMessageDispatcher dispatcher(repo, repo.interface(), repo.plugin());
		MainWindow           main_window(repo, dispatcher);
		DumpHandler          dumper([&]()
		{
			ModelRailway * model = repo;

			model->info();
		});

		repo.info();
		repo.xml();

		main_window.show();

		qInfo("==========================================================");
		qInfo() << "MRW-NG track control using model:" << repo.modelName();
		qInfo("==========================================================");
		return app.exec();
	}
	else
	{
		qCritical().noquote() << "No model available:" << repo.modelName();

		return EXIT_FAILURE;
	}
}
