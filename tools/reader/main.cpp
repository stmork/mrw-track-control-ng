//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/duration.h>
#include <model/modelrailway.h>
#include <model/abstractswitch.h>
#include <model/signal.h>

using namespace mrw::util;
using namespace mrw::model;

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	Duration::pattern();
	for (int i = 1; i < argc; i++)
	{
		ModelRailway model(argv[i]);
		const MrwStatistic & statistics = model.statistics();

		qInfo("===== Modelrailway: %s", argv[i]);
		qInfo("Regions:       %3zu", statistics.region_count);
		qInfo("Devices:       %3zu", statistics.device_count);
		qInfo("Sections:      %3zu", statistics.section_count);
		qInfo("Switches:      %3zu", statistics.switch_count);
		qInfo("Signals:       %3zu", statistics.signal_count);
		qInfo("Signal groups: %3zu", statistics.signal_group_count);
		qInfo("Main signals:  %3zu", statistics.main_signal_count);
		model.info();
	}
	return EXIT_SUCCESS;
}
