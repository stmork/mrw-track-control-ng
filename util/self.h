//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_SELF_H
#define MRW_UTIL_SELF_H

#include <memory>

namespace mrw::util
{
	/**
	 * This template class encapsulates an already existing instance into
	 * a std::shared_ptr. On destruction the instance is not deleted since the
	 * deleter does intentionally nothing.
	 */
	template<class T> class SelfPointer
	{
	public:
		SelfPointer() = delete;
		explicit SelfPointer(T * instance) : self(instance, &null_deleter)
		{
		}

		/**
		 * This cast operator returns a reference to the std::shared_ptr
		 * of this instance.
		 */
		inline operator std::shared_ptr<T> & ()
		{
			return self;
		}

	private:
		std::shared_ptr<T> self;

		static void null_deleter(T * ptr)
		{
			(void)ptr;
		}
	};
}

#endif
