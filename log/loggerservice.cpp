//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <util/duration.h>
#include <log/loggerservice.h>

using namespace mrw::util;
using namespace mrw::log;

LoggerService::LoggerService()
{
	Duration::pattern(false);
	default_handler = qInstallMessageHandler(log);
}

void LoggerService::registerLogger(LoggerBase * logger)
{
	logger_list.push_back(logger);
}

void LoggerService::unregisterLogger(LoggerBase * logger)
{
	logger_list.remove(logger);
}

void LoggerService::log(
	QtMsgType                  type,
	const QMessageLogContext & context,
	const QString       &      input)
{
	const std::list<LoggerBase *> & logger_list = LoggerService::instance().logger_list;

	if (logger_list.empty())
	{
		LoggerService::instance().default_handler(type, context, input);
		return;
	}

	const std::string & formatted = qFormatLogMessage(type, context, input).toStdString();
	const char     *     msg      = formatted.c_str();

	switch (type)
	{
	case QtDebugMsg:
		for (LoggerBase * logger : logger_list)
		{
			logger->debug(msg);
		}
		break;

	case QtInfoMsg:
		for (LoggerBase * logger : logger_list)
		{
			logger->info(msg);
		}
		break;

	case QtWarningMsg:
		for (LoggerBase * logger : logger_list)
		{
			logger->warn(msg);
		}
		break;

	case QtCriticalMsg:
		for (LoggerBase * logger : logger_list)
		{
			logger->critical(msg);
		}
		break;

	case QtFatalMsg:
		for (LoggerBase * logger : logger_list)
		{
			logger->fatal(msg);
		}
		break;
	}
}
