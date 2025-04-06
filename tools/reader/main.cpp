//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>
#include <QLoggingCategory>

#include <util/duration.h>
#include <model/modelrailway.h>
#include <model/abstractswitch.h>
#include <model/signal.h>

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);
	QLoggingCategory log("mrw.tools.reader");

	Duration::pattern();
	for (int i = 1; i < argc; i++)
	{
		ModelRailway model(argv[i]);
		const MrwStatistic & statistics = model.statistics();

		qCInfo(log, "===== Modelrailway: %s", argv[i]);
		qCInfo(log, "Regions:       %3zu", statistics.region_count);
		qCInfo(log, "Devices:       %3zu", statistics.device_count);
		qCInfo(log, "Sections:      %3zu", statistics.section_count);
		qCInfo(log, "Switches:      %3zu", statistics.switch_count);
		qCInfo(log, "Signals:       %3zu", statistics.signal_count);
		qCInfo(log, "Signal groups: %3zu", statistics.signal_group_count);
		qCInfo(log, "Main signals:  %3zu", statistics.main_signal_count);
		model.info();
	}
	return EXIT_SUCCESS;
}
