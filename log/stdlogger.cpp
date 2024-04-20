//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <log/stdlogger.h>

using namespace mrw::log;

void StdLogger::debug(const char * message) const
{
	write(stdout, message);
}

void StdLogger::info(const char * message) const
{
	write(stdout, message);
}

void StdLogger::warn(const char * message) const
{
	write(stderr, message);
}

void StdLogger::critical(const char * message) const
{
	write(stderr, message);
}

void StdLogger::fatal(const char * message) const
{
	write(stderr, message);
}

void StdLogger::write(FILE * file, const char * message)
{
	const std::string & now = timeStamp().toStdString();

	fprintf(file, "%s %s\n", now.c_str(), message);
	fflush(file);
}
