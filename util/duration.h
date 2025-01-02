//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_DURATION_H
#define MRW_UTIL_DURATION_H

#include <QElapsedTimer>

namespace mrw::util
{
	class Duration
	{
		QElapsedTimer timer;
		const char  * name;

	public:
		static const QString TIME_STAMP_FORMAT;

		/**
		 * This method sets the logging information to a unique pattern with
		 * timestamp logging type and of course its message.
		 *
		 * @param use_time_stamp If true add a time stamp placeholder.
		 */
		static void pattern(const bool use_time_stamp = true) noexcept;

		/**
		 * The constructor logs the entry of a method.
		 *
		 * @param method_name The methods name. Use the \c __FUNCTION__ macro here.
		 */
		explicit Duration(const char * naming);

		/**
		 * The destructor logs the exit of a method and the time spent inside
		 * the method.
		 */
		virtual ~Duration();
	};
}

#endif
