//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_LOG_FILELOGGER_H
#define MRW_LOG_FILELOGGER_H

#include <QFile>

#include <log/loggerbase.h>

namespace mrw::log
{
	/**
	 * This class writes all logging messages into one single file.
	 */
	class FileLogger : public UnifiedLogger
	{
		mutable QFile file;

	public:
		explicit FileLogger(const QString & filename = "application.log");
		~FileLogger();

	protected:
		void write(const char * message) const override;
	};

	/**
	 * This class writes all logging messages into one single file. The filename
	 * is built from the date time stamp and a prefix to distinguish different
	 * applications.
	 */
	class TimestampedFileLogger : public FileLogger
	{
	public:
		explicit TimestampedFileLogger(const QString & prefix = "application");

	private:
		static QString filename(const QString & prefix);
	};
}

#endif
