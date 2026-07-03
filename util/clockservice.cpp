//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <QTime>

#include "clockservice.h"

using namespace mrw::util;

ClockService::ClockService() : QObject(nullptr)
{
	timer.setInterval(125);
	timer.setSingleShot(false);
	timer.setTimerType(Qt::PreciseTimer);

	connect(&timer, &QTimer::timeout, this, &ClockService::tick);

	const int millies = QTime::currentTime().msec();

	QTimer::singleShot(1005 - millies, this, [this]()
	{
		tick_counter = QTime::currentTime().msec() / 125;

		timer.start();
	});
}

void ClockService::tick()
{
	tick_counter++;

	emit Hz8(tick_counter);

	if ((tick_counter & 0b001) == 0)
	{
		emit Hz4(tick_counter >> 1);
	}

	if ((tick_counter & 0b011) == 0)
	{
		emit Hz2(tick_counter >> 2);
	}

	if ((tick_counter & 0b111) == 0)
	{
		emit Hz1(tick_counter >> 3);
	}
}
