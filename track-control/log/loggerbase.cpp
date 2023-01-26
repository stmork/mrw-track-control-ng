//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <log/loggerservice.h>
#include <log/loggerbase.h>

using namespace mrw::log;

LoggerBase::~LoggerBase()
{
	LoggerService::instance().unregisterLogger(this);
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
