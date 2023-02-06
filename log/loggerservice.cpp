//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <util/method.h>
#include <log/loggerservice.h>

using namespace mrw::util;
using namespace mrw::log;

LoggerService::LoggerService()
{
	Method::pattern();
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

	QByteArray   bytes = qFormatLogMessage(type, context, input).toLocal8Bit();
	const char * msg   = bytes.constData();

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
