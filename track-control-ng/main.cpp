//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include "mainwindow.h"
#include "model/modelrailway.h"

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);
	QDir         home = QDir::homePath();
	QSettings    settings("mrw", "model");
	QString      filename = argc > 1 ?
		argv[1] :
		settings.value("model/filename",
			home.filePath("mrw/RailwayModel.modelrailway")).toString();

	if (QFile::exists(filename))
	{
		mrw::model::ModelRailway     model(filename);

//		model.dump();
		model.info();
		settings.setValue("model/filename", filename);
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
