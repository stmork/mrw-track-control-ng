//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <fstream>
#include <chrono>

#include <QCoreApplication>
#include <QHostInfo>

#include "appsupport.h"

using namespace std::chrono;
using namespace std::chrono_literals;

using namespace mrw::util;

AppSupport::AppSupport()
{
	QCoreApplication::setOrganizationName("mrw");
	QCoreApplication::setOrganizationDomain("org.eisenbahnsteuerung");
}

AppSupport & AppSupport::instance() noexcept
{
	static AppSupport app_support;

	return app_support;
}

QString AppSupport::hostname() const noexcept
{
	return QHostInfo::localHostName().split(".").at(0).toLower();
}

unsigned AppSupport::blanktime() const noexcept
{
	static constexpr seconds min_blank_time = 5min;
	static constexpr seconds max_blank_time = 6h;
	std::fstream             blank_file("/sys/module/kernel/parameters/consoleblank", std::ios_base::in);
	auto                     blank_time = min_blank_time.count();

	blank_file >> blank_time;

	return std::clamp(blank_time, min_blank_time.count(), max_blank_time.count());
}
