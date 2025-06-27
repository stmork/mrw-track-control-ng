//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QDebug>

#ifdef USE_SYSTEMD
#include <systemd/sd-daemon.h>
#endif

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
#include "log.h"

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
			qCInfo(mrw::tools::log).noquote() << "Window size: " << main_window.size();

			qCInfo(mrw::tools::log, "==========================================================");
			qCInfo(mrw::tools::log) << "MRW-NG track control using model:" << repo.modelName();
#ifdef VERSION
			qCInfo(mrw::tools::log)           << "Version:   " << VERSION;
#endif
#ifdef BUILD_NUMBER
			qCInfo(mrw::tools::log)           << "Build:     " << BUILD_NUMBER;
#endif
			qCInfo(mrw::tools::log)           << "Qt version:" << qVersion();
			qCInfo(mrw::tools::log).noquote() << "CAN plugin:" << repo.plugin();
			qCInfo(mrw::tools::log).noquote() << "CAN iface: " << repo.interface();
			qCInfo(mrw::tools::log, "==========================================================");
#ifdef USE_SYSTEMD
			sd_notify(0, "READY=1");
#endif
			return app.exec();
		}
		catch (const std::exception & exception)
		{
			dispatcher.emergencyStop();
			qCCritical(mrw::tools::log, "FATAL: %s", exception.what());
		}
	}
	else
	{
		qCCritical(mrw::tools::log).noquote() << "No model available:" << repo.modelName();

		return EXIT_FAILURE;
	}
}
