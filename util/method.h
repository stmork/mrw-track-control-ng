//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_METHOD_H
#define MRW_UTIL_METHOD_H

#include <QElapsedTimer>

namespace mrw::util
{

#define __METHOD__ mrw::util::Method __method__(__FUNCTION__)

	/**
	 * This convenience class may be used for logging the entry, exit and
	 * time spent of a method. Use the __METHOD__ macro at the very first
	 * of a method and the logging occurs automagically.
	 * @note Using the __METHOD__ macro requires your class to derive from
	 * Logging.
	 */
	class Method
	{
		QElapsedTimer timer;
		const char  * method_name;

	public:
		/**
		 * The constructor logs the entry of a method.
		 *
		 * @param method_name The methods name. Use the \c __FUNCTION__ macro here.
		 */
		Method(const char * method_name);

		/**
		 * The destructor logs the exit of a method and the time spent inside
		 * the method.
		 */
		virtual ~Method();
	};
}

#endif
