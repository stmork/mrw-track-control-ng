//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/method.h>
#include <model/modelrailway.h>
#include <model/abstractswitch.h>
#include <model/signal.h>

using namespace mrw::util;
using namespace mrw::model;

template<class T> static size_t count(
	ModelRailway & model,
	std::function<bool(const T *)> guard = &Method::always<T>)
{
	std::vector<T *> result;

	model.parts<T>(result, guard);
	return result.size();
}

static size_t countSignalGroups(const Section * section)
{
	size_t signal_group_count = 0;

	if (section->getSignals(false).size() > 0)
	{
		signal_group_count++;
	}
	if (section->getSignals(true).size() > 0)
	{
		signal_group_count++;
	}
	return signal_group_count;
}

int main(int argc, char * argv[])
{
	QCoreApplication app(argc, argv);

	Method::pattern();
	for (int i = 1; i < argc; i++)
	{
		ModelRailway model(argv[i]);
		size_t       region_count       = model.regionCount();
		size_t       section_count      = 0;
		size_t       signal_group_count = 0;

		for (size_t r = 0; r < region_count; r++)
		{
			const Region * region = model.region(r);

			section_count += region->sectionCount();
			for (size_t s = 0; s < region->sectionCount(); s++)
			{
				signal_group_count += countSignalGroups(region->section(s));
			}
		}

		qInfo("===== Modelrailway: %s", argv[i]);
		qInfo("Regions:       %3zu", region_count);
		qInfo("Sections:      %3zu", section_count);
		qInfo("Switches:      %3zu", count<AbstractSwitch>(model));
		qInfo("Signals:       %3zu", count<Signal>(model));
		qInfo("Signal groups: %3zu", signal_group_count);
		qInfo("Main signals:  %3zu", count<Signal>(model, [](const Signal * signal)
		{
			return (signal->type() & Signal::MAIN_SIGNAL) != 0;
		}));
		model.info();
	}
	return EXIT_SUCCESS;
}
