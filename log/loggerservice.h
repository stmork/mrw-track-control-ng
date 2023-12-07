//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_LOG_LOGGERSERVICE_H
#define MRW_LOG_LOGGERSERVICE_H

#include <list>

#include <QtDebug>

#include <util/singleton.h>
#include <log/loggerbase.h>

/**
 * The mrw::log namespace provieds classes vor convenient Qt logging. You can
 * register multiple LoggerBase instances into the LoggerService singleton.
 * The logging methods of each LoggingBase instance is called in the order of
 * their registration.
 *
 * <img src="log-diagram.png"/>
 */
namespace mrw::log
{
	/**
	 * This singleton provides the handling of multiple LoggerBase instances.
	 * The LoggerBase instances are collected into a list and each of the
	 * instances are called according to the QtMsgType. The constructor also
	 * calls mrw::util::Method::pattern() for time stamping.
	 */
	class LoggerService : public mrw::util::Singleton<LoggerService>
	{
		QtMessageHandler        default_handler;
		std::list<LoggerBase *> logger_list;

		LoggerService();

	public:
		friend class Singleton<LoggerService>;

		/**
		 * This method registers a LoggerBase instance.
		 *
		 * @param logger The LoggerBase instance.
		 */
		void registerLogger(LoggerBase * logger);

		/**
		 * This method unregisters a LoggerBase instance.
		 *
		 * @param logger The LoggerBase instance.
		 */
		void unregisterLogger(LoggerBase * logger);

	private:
		/**
		 * This is the message handler installed into this service. If the
		 * logger list is empty the previous installed logger is used. Otherwise
		 * the callback of each listed LoggerBase is called according to its
		 * QtMsgType.
		 *
		 * @param type The logging type
		 * @param context The context which may contain source code information.
		 * @param input The message itself.
		 *
		 * @see https://doc.qt.io/qt-6/qtglobal.html#qInstallMessageHandler
		 */
		static void log(
			QtMsgType                  type,
			const QMessageLogContext & context,
			const QString       &      input);
	};
}

#endif
