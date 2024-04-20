//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <log/syslogger.h>

#include <syslog.h>

using namespace mrw::log;

SysLogger::SysLogger(const char * application)
{
	openlog(application, LOG_NDELAY, LOG_USER);
}

SysLogger::~SysLogger()
{
	closelog();
}

void SysLogger::debug(const char * message) const
{
	write(LOG_DEBUG, message);
}

void SysLogger::info(const char * message) const
{
	write(LOG_INFO, message);
}

void SysLogger::warn(const char * message) const
{
	write(LOG_WARNING, message);
}

void SysLogger::critical(const char * message) const
{
	write(LOG_ERR, message);
}

void SysLogger::fatal(const char * message) const
{
	write(LOG_CRIT, message);
}

void SysLogger::write(
	const int    priority,
	const char * message) const
{
	syslog(priority, "%s", message);
}
