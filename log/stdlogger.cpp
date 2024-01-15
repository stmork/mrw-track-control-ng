//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <stdio.h>

#include <log/stdlogger.h>

using namespace mrw::log;

void StdLogger::debug(const char * message) const
{
	fprintf(stdout, "%s\n", message);
	fflush(stdout);
}

void StdLogger::info(const char * message) const
{
	fprintf(stdout, "%s\n", message);
	fflush(stdout);
}

void StdLogger::warn(const char * message) const
{
	fprintf(stderr, "%s\n", message);
	fflush(stderr);
}

void StdLogger::critical(const char * message) const
{
	fprintf(stderr, "%s\n", message);
	fflush(stderr);
}

void StdLogger::fatal(const char * message) const
{
	fprintf(stderr, "%s\n", message);
	fflush(stderr);
}
