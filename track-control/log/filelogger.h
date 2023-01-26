//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_LOG_FILELOGGER_H
#define MRW_LOG_FILELOGGER_H

#include <QFile>

#include <log/loggerbase.h>

namespace mrw::log
{
	class FileLogger : public UnifiedLogger
	{
		mutable QFile file;

	public:
		explicit FileLogger(const QString & filename = "application.log");
		~FileLogger();

	protected:
		void write(const char * message) const override;
	};

	class TimestampedFileLogger : public FileLogger
	{
	public:
		explicit TimestampedFileLogger(const QString & prefix = "application");

	private:
		static QString filename(const QString & prefix);
	};
}

#endif
