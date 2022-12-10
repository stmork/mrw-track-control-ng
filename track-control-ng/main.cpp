//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include <util/settings.h>
#include <util/dumphandler.h>
#include <util/termhandler.h>
#include <model/modelrailway.h>

#include "mainwindow.h"
#include "modelrepository.h"
#include "mrwmessagedispatcher.h"

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QApplication  app(argc, argv);
	TermHandler   terminator( { SIGTERM, SIGINT } );
	Settings      settings("model");
	SettingsGroup group(&settings, "model");
	QString       modelname = argc > 1 ?
		argv[1] :
		settings.value("filename", "RailwayModel").toString();
	ModelRepository repo(modelname);

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

		settings.setValue("filename", modelname);
		main_window.show();
		return app.exec();
	}
	else
	{
		qCritical() << "No model available: " << modelname;

		return EXIT_FAILURE;
	}
}
