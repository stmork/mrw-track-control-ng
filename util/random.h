//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
		std::mt19937        random_number_generator;

		friend class Singleton<Random>;

		Random();

	public:
		/**
		 * This static access method returns the random generator engine.
		 *
		 * @return The random generator engine needed for distributions.
		 */
		inline std::mt19937 & engine()
		{
			return random_number_generator;
		}

		/**
		 * This convenience method returns a secure random number in the
		 * range [0..size[.
		 *
		 * @param size The maximum border of the random number range excluding
		 * the maximum.
		 * @return The secure random number.
		 */
		static int random(const size_t size);

		/**
		 * This static template method returns a secure random number of the
		 * specified type and range [0..max].
		 *
		 * @param size The maximum border of the random number range including
		 * the maximum.
		 * @return The secure random number.
		 */
		template<typename T> inline static T random(const T max)
		{
			std::uniform_int_distribution<T> distribution(0, max);

			return distribution(instance().engine());
		}
	};
}

#endif
