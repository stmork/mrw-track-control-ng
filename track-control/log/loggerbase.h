//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_LOG_LOGGERBASE_H
#define MRW_LOG_LOGGERBASE_H

namespace mrw::log
{
	class LoggerBase
	{
	public:
		virtual ~LoggerBase();

		virtual void debug(const char * message) const = 0;
		virtual void info(const char * message) const = 0;
		virtual void warn(const char * message) const = 0;
		virtual void critical(const char * message) const = 0;
		virtual void fatal(const char * message) const = 0;
	};

	class UnifiedLogger : public LoggerBase
	{
	protected:
		virtual void write(const char * message) const = 0;

	private:
		void debug(const char * message) const override;
		void info(const char * message) const override;
		void warn(const char * message) const override;
		void critical(const char * message) const override;
		void fatal(const char * message) const override;
	};
}

#endif
