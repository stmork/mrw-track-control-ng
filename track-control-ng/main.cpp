//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include "mainwindow.h"
#include <model/modelrailway.h>
#include <util/settings.h>

using namespace mrw::util;

int main(int argc, char * argv[])
{
	QApplication  app(argc, argv);
	QDir          home = QDir::homePath();
	Settings      settings("model");
	SettingsGroup group(&settings, "model");
	QString       filename = argc > 1 ?
		argv[1] :
		settings.value("filename",
			home.filePath("mrw/RailwayModel.modelrailway")).toString();

	if (QFile::exists(filename))
	{
		mrw::model::ModelRailway     model(filename);

//		model.dump();
		model.info();
		settings.setValue("filename", filename);
	}
	else
	{
		qCritical() << "No model available: " << filename;
	}

#if 0
	MainWindow   main_window;

	main_window.show();
	return app.exec();
#else
	return EXIT_SUCCESS;
#endif
}
