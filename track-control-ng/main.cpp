//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include <util/settings.h>
#include <util/termhandler.h>
#include <model/modelrailway.h>

#include "mainwindow.h"
#include "modelrepository.h"

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
		ModelRailway * model = repo;

//		model->dump();
		model->info();
		settings.setValue("filename", modelname);
	}
	else
	{
		qCritical() << "No model available: " << modelname;
	}

#if 1
	MainWindow   main_window(repo);

	main_window.show();
	return app.exec();
#else
	return EXIT_SUCCESS;
#endif
}
