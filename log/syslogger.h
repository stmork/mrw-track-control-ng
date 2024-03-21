//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_LOG_SYSLOGGER_H
#define MRW_LOG_SYSLOGGER_H

#include <log/loggerbase.h>

namespace mrw::log
{
	class SysLogger : public LoggerBase
	{
	public:
		SysLogger() = delete;
		explicit SysLogger(const char * application);
		virtual ~SysLogger();

		virtual void debug(const char * message) const override;
		virtual void info(const char * message) const override;
		virtual void warn(const char * message) const override;
		virtual void critical(const char * message) const override;
		virtual void fatal(const char * message) const override;

	private:
		void write(const int pririty, const char * message) const;
	};
}

#endif
