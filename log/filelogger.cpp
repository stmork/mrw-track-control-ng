//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QDateTime>

#include <log/filelogger.h>

using namespace mrw::log;

FileLogger::FileLogger(const QString & filename) : file(filename)
{
	file.open(QIODevice::WriteOnly | QIODevice::Text);
}

FileLogger::~FileLogger()
{
	file.close();
}

void FileLogger::write(const char * message) const
{
	const std::string & now = timeStamp().toStdString();

	file.write(now.c_str());
	file.write(" ");
	file.write(message);
	file.write("\n");
	file.flush();
}

TimestampedFileLogger::TimestampedFileLogger(const QString & prefix) :
	FileLogger(filename(prefix))
{
}

QString TimestampedFileLogger::filename(const QString & prefix)
{
	QDateTime now = QDateTime::currentDateTime();

	return prefix + "_" + now.toString("yyyyMMdd_HHmmss") + ".log";
}
