//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_METHOD_H
#define MRW_UTIL_METHOD_H

#include <QDebug>
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
		explicit Method(const char * method_name);

		/**
		 * The destructor logs the exit of a method and the time spent inside
		 * the method.
		 */
		virtual ~Method();

		/**
		 * This method sets the logging information to a unique pattern with
		 * timestamp logging type and of course its message.
		 */
		static void pattern() noexcept;

		/**
		 * This static template function acts as a default for lambda
		 * callbacks which always returns true on a specific given type T.
		 *
		 * @param ptr Never used.
		 * @returns Always true regarding to input.
		 */
		template<class T> static bool always(const T * ptr) noexcept
		{
			(void)ptr;

			return true;
		}

		/**
		 * This static template function acts as a default for lambda
		 * callbacks which always returns false on a specific given type T.
		 *
		 * @param ptr Never used.
		 * @returns Always false regarding to input.
		 */
		template<class T> static bool never(const T * ptr) noexcept
		{
			(void)ptr;

			return false;
		}

		/**
		 * The static template function acts as a default for lambda or
		 * singleton callbacks which simply does nothing on a specific given
		 * type T.
		 *
		 * @param ptr The pointer which is not used.
		 */
		template<class T> static void noop(const T * ptr)
		{
			(void)ptr;
		}
	};
}

#endif
