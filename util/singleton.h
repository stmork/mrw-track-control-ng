//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_SINGLETON_H
#define MRW_UTIL_SINGLETON_H

#include <type_traits>

namespace mrw::util
{
	/**
	 * This template class implements the singleton pattern as a base class
	 * for real implementations.
	 *
	 * @attention Do not derive this class more than once since the
	 * instance() method will only work with the first derived class.
	 */
	template <class T> class Singleton
	{
	protected:
		/**
		 * The default constructor should only be accessible to derviced
		 * classes. Derived singleton classes should look like the following
		 * example:
		 *
		 * @code
		 * class ExampleSingleton : public Singleton<ExampleSingleton>
		 * {
		 * 	// private default constructor!!!
		 * 	ExampleSingleton() = default;
		 *
		 * 	friend class Singleton<ExampleSingleton>;
		 * public:
		 * 	// ... any accessible members.
		 * };
		 * @endcode
		 *
		 * @note The default constructor of the derived class should be
		 * private to meet the singleton pattern.
		 */
		Singleton() = default;

	public:
		Singleton(const Singleton & other) = delete;
		Singleton & operator=(const Singleton & other) = delete;

		/**
		 * This static method returns the singleton class instance.
		 *
		 * @return The singleton class instance.
		 */
		[[nodiscard]]
		inline static T & instance() noexcept
		{
			static_assert(std::is_base_of<Singleton, T>(), "Template class is not a singleton!");

			static T singleton;

			return singleton;
		}
	};
}

#endif
