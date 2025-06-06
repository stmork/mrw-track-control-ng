//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_CLEANVECTOR_H
#define MRW_UTIL_CLEANVECTOR_H

#include <vector>
#include <memory>

namespace mrw::util
{
	/**
	 * This class is derived from std::vector and provides an auto delete
	 * functionality of the contained pointer types upon destruction.
	 */
	template<class TYPE>
	class CleanVector : public std::vector<TYPE *>
	{
	public:
		inline virtual ~CleanVector()
		{
			for (TYPE * part : *this)
			{
				delete part;
			}
			this->clear();
		}
	};

	/**
	 * This template class wraps a type into a shared pointer used for a
	 * std::vector.
	 */
	template<class TYPE>
	class SharedVector : public std::vector<std::shared_ptr<TYPE>>
	{
	public:
		using PtrType = std::shared_ptr<TYPE>;
	};
}

#endif
