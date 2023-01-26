//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_LOG_STDLOGGER_H
#define MRW_LOG_STDLOGGER_H

#include <log/loggerbase.h>

namespace mrw::log
{
	class StdLogger : public LoggerBase
	{
	public:
		virtual void debug(const char * message) const override;
		virtual void info(const char * message) const override;
		virtual void warn(const char * message) const override;
		virtual void critical(const char * message) const override;
		virtual void fatal(const char * message) const override;
	};
}

#endif
