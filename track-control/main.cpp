//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include <systemd/sd-daemon.h>

#include <util/method.h>
#include <util/settings.h>
#include <util/dumphandler.h>
#include <model/modelrepository.h>
#include <log/stdlogger.h>
#include <log/filelogger.h>
#include <log/syslogger.h>
#include <log/loggerservice.h>

#include "mainwindow.h"
#include "mrwmessagedispatcher.h"

using namespace mrw::util;
using namespace mrw::log;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QApplication             app(argc, argv);
	StdLogger                std_logger;
	TimestampedFileLogger    file_logger("track-control");
	SysLogger                sys_logger("track-control");

	LoggerService::instance().registerLogger(&std_logger);
	LoggerService::instance().registerLogger(&file_logger);
	LoggerService::instance().registerLogger(&sys_logger);

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

		try
		{
			qInfo().noquote() << "Window size: " << main_window.size();

			qInfo("==========================================================");
			qInfo() << "MRW-NG track control using model:" << repo.modelName();
#ifdef VERSION
			qInfo()           << "Version:   " << VERSION;
#endif
#ifdef BUILD_NUMBER
			qInfo()           << "Build:     " << BUILD_NUMBER;
#endif
			qInfo()           << "Qt version:" << qVersion();
			qInfo().noquote() << "CAN plugin:" << repo.plugin();
			qInfo().noquote() << "CAN iface: " << repo.interface();
			qInfo("==========================================================");
			sd_notify(0, "READY=1");
			return app.exec();
		}
		catch (const std::exception & exception)
		{
			dispatcher.emergencyStop();
			qCritical("FATAL: %s", exception.what());
		}
	}
	else
	{
		qCritical().noquote() << "No model available:" << repo.modelName();

		return EXIT_FAILURE;
	}
}
