//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_METHOD_H
#define MRW_UTIL_METHOD_H

#include <util/duration.h>

namespace mrw::util
{

#	define __METHOD__ mrw::util::Duration __method__(__FUNCTION__)

	/**
	 * This convenience class may be used for logging the entry, exit and
	 * time spent of a method. Use the __METHOD__ macro at the very first
	 * of a method and the logging occurs automagically.
	 * @note Using the __METHOD__ macro requires your class to derive from
	 * Logging.
	 */
	class Method
	{
	public:
		/**
		 * This static template function acts as a default for lambda
		 * callbacks which always returns true on a specific given type T.
		 *
		 * @param ptr Never used.
		 * @returns Always true regarding to input.
		 */
		template<class T>
		[[nodiscard]]
		static constexpr bool always(const T * ptr) noexcept
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
		template<class T>
		[[nodiscard]]
		static constexpr bool never(const T * ptr) noexcept
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
		template<class T> static constexpr void noop(const T * ptr)
		{
			(void)ptr;
		}
	};
}

#endif
