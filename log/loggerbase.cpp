//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QDateTime>

#include <util/duration.h>
#include <log/loggerservice.h>
#include <log/loggerbase.h>

using namespace mrw::util;
using namespace mrw::log;

LoggerBase::~LoggerBase()
{
	LoggerService::instance().unregisterLogger(this);
}

QString LoggerBase::timeStamp() noexcept
{
	QDateTime now = QDateTime::currentDateTime();

	return now.toString(Duration::TIME_STAMP_FORMAT);
}

void UnifiedLogger::debug(const char * message) const
{
	write(message);
}

void UnifiedLogger::info(const char * message) const
{
	write(message);
}

void UnifiedLogger::warn(const char * message) const
{
	write(message);
}

void UnifiedLogger::critical(const char * message) const
{
	write(message);
}

void UnifiedLogger::fatal(const char * message) const
{
	write(message);
}
