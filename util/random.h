//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_RANDOM_H
#define MRW_UTIL_RANDOM_H

#include <random>

#include <util/singleton.h>

namespace mrw::util
{
	/**
	 * This singleton class provides secure random generation needed for
	 * crypto key generation. The usage is quiet simple. You need a
	 * distribution function you need. Mostly you need the typed uniform
	 * distribution for integers implemented in the
	 * std::uniform_int_distribution template class. You can define the
	 * borders of the random integer range. The constructor default values
	 * select automatically the possible type type maximum.
	 *
	 * @note Read more about random number generation in C++ standard
	 * documention.
	 *
	 * Usage Example:
	 * @code
	 * std::uniform_int_distribution<uint32_t> dist_ssrc;
	 * ...
	 * uint32_t ssrc = dist_ssrc(Random::instance().engine());
	 * @endcode
	 *
	 * @see https://en.cppreference.com/w/cpp/numeric/random
	 * @see http://www.cplusplus.com/reference/random/
	 */
	class Random : public Singleton<Random>
	{
		std::random_device  random_device;

		friend class Singleton<Random>;

	public:
		/**
		 * This access method returns the random device.
		 *
		 * @return The random device needed for distributions.
		 */
		[[nodiscard]]
		constexpr std::random_device & engine() noexcept
		{
			return random_device;
		}

		/**
		 * This static template method returns a secure random number of the
		 * specified type and range [0..max].
		 *
		 * @param max The maximum border of the random number range including
		 * the maximum.
		 * @return The secure random number.
		 */
		template<typename T>
		[[nodiscard]]
		constexpr static T random(const T max) noexcept
		{
			std::uniform_int_distribution<T> distribution(0, max);

			return distribution(instance().engine());
		}
	};
}

#endif
