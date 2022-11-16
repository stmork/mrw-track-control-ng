//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include <util/settings.h>
#include <model/modelrailway.h>

#include "mainwindow.h"
#include "modelrepository.h"

using namespace mrw::util;

int main(int argc, char * argv[])
{
	QApplication  app(argc, argv);
	Settings      settings("model");
	SettingsGroup group(&settings, "model");
	QString       modelname = argc > 1 ?
		argv[1] :
		settings.value("filename", "RailwayModel").toString();
	ModelRepository repo(modelname);

	if (repo)
	{
		mrw::model::ModelRailway * model = repo;

//		model->dump();
		model->info();
		settings.setValue("filename", modelname);
	}
	else
	{
		qCritical() << "No model available: " << modelname;
	}

#if 1
	MainWindow   main_window;

	main_window.show();
	return app.exec();
#else
	return EXIT_SUCCESS;
#endif
}
