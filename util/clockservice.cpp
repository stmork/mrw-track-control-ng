//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "clockservice.h"

using namespace mrw::util;

ClockService::ClockService() : QObject(nullptr)
{
	timer_1hz.setInterval(1000);
	timer_1hz.setSingleShot(false);
	timer_2hz.setInterval(500);
	timer_2hz.setSingleShot(false);
	timer_4hz.setInterval(250);
	timer_4hz.setSingleShot(false);
	timer_8hz.setInterval(125);
	timer_8hz.setSingleShot(false);

	connect(&timer_1hz, &QTimer::timeout, this, &ClockService::Hz1);
	connect(&timer_2hz, &QTimer::timeout, this, &ClockService::Hz2);
	connect(&timer_4hz, &QTimer::timeout, this, &ClockService::Hz4);
	connect(&timer_8hz, &QTimer::timeout, this, &ClockService::Hz8);

	timer_1hz.start();
	timer_2hz.start();
	timer_4hz.start();
	timer_8hz.start();
}
