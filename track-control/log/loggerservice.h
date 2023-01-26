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

namespace mrw::log
{
	class LoggerService : public mrw::util::Singleton<LoggerService>
	{
		std::list<LoggerBase *> logger_list;

		LoggerService();

	public:
		friend class Singleton<LoggerService>;

		void registerLogger(LoggerBase * logger);
		void unregisterLogger(LoggerBase * logger);

	private:
		static void log(
			QtMsgType                  type,
			const QMessageLogContext & context,
			const QString       &      input);
	};
}

#endif
