//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_LOG_STDLOGGER_H
#define MRW_LOG_STDLOGGER_H

#include <cstdio>

#include <log/loggerbase.h>

namespace mrw::log
{
	/**
	 * This class implements logging to console. The debug and info messages
	 * are logged to stdout and the higher log levels are logged to stderr.
	 */
	class StdLogger : public LoggerBase
	{
	public:
		virtual void debug(const char * message) const override;
		virtual void info(const char * message) const override;
		virtual void warn(const char * message) const override;
		virtual void critical(const char * message) const override;
		virtual void fatal(const char * message) const override;

	protected:
		/**
		 * The callback method for all other logging methods of StdLogger
		 * classes. This is useful to write all messages into different
		 * file handles.
		 *
		 * @param fd The FILE pointer to log into.
		 * @param message The message to log.
		 * @see FileLogger
		 */
		static void write(FILE * file, const char * message);
	};
}

#endif
