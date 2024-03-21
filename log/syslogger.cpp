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

mrw::log::SysLogger::~SysLogger()
{
	closelog();
}

void mrw::log::SysLogger::debug(const char * message) const
{
	write(LOG_DEBUG, message);
}

void mrw::log::SysLogger::info(const char * message) const
{
	write(LOG_INFO, message);
}

void mrw::log::SysLogger::warn(const char * message) const
{
	write(LOG_WARNING, message);
}

void mrw::log::SysLogger::critical(const char * message) const
{
	write(LOG_ERR, message);
}

void mrw::log::SysLogger::fatal(const char * message) const
{
	write(LOG_CRIT, message);
}

void mrw::log::SysLogger::write(
	const int    priority,
	const char * message) const
{
	syslog(priority, "%s", message);
}
