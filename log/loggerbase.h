//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_LOG_LOGGERBASE_H
#define MRW_LOG_LOGGERBASE_H

#include <QString>

namespace mrw::log
{
	/**
	 * This class provides an interface for logging messages into different
	 * drains. The callbacks are already sorted accoring to the QtMsgType.
	 */
	class LoggerBase
	{
	public:
		virtual ~LoggerBase();

		/**
		 * This method is called on logging if the QtMsgType is QtDebugMsg.
		 * Overload to write the message into a drain on your demand.
		 *
		 * @param message The message to log.
		 */
		virtual void debug(const char * message) const = 0;

		/**
		 * This method is called on logging if the QtMsgType is QtInfoMsg.
		 * Overload to write the message into a drain on your demand.
		 *
		 * @param message The message to log.
		 */
		virtual void info(const char * message) const = 0;

		/**
		 * This method is called on logging if the QtMsgType is QtWarningMsg.
		 * Overload to write the message into a drain on your demand.
		 *
		 * @param message The message to log.
		 */
		virtual void warn(const char * message) const = 0;

		/**
		 * This method is called on logging if the QtMsgType is QtCriticalMsg.
		 * Overload to write the message into a drain on your demand.
		 *
		 * @param message The message to log.
		 */
		virtual void critical(const char * message) const = 0;

		/**
		 * This method is called on logging if the QtMsgType is QtFatalMsg.
		 * Overload to write the message into a drain on your demand.
		 *
		 * @param message The message to log.
		 */
		virtual void fatal(const char * message) const = 0;

	protected:
		/**
		 * This method creates a time stamp of the actual date time for logging.
		 *
		 * @return The actual date time as QString.
		 */
		static QString timeStamp() noexcept;
	};

	/**
	 * This method unifies all log messages into one single callback.
	 */
	class UnifiedLogger : public LoggerBase
	{
	protected:
		/**
		 * The callback method for all other logging methods of LoggerBase
		 * classes. This is useful to write all messages into one file.
		 *
		 * @param message The message to log.
		 * @see FileLogger
		 */
		virtual void write(const char * message) const = 0;

	private:
		void debug(const char * message) const override;
		void info(const char * message) const override;
		void warn(const char * message) const override;
		void critical(const char * message) const override;
		void fatal(const char * message) const override;
	};
}

#endif
