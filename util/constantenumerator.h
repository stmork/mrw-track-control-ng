//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_CONSTANTENUMERATOR_H
#define MRW_UTIL_CONSTANTENUMERATOR_H

#include <unordered_map>

#include <QString>

#define CONSTANT(f) { f, #f }

namespace mrw::util
{
	/**
	 * This convenience class provides a mapping from a symbolic
	 * value to its string representation. It is used for printing
	 * clear text value while debugging.
	 *
	 * The following example shows how to map error codes to their
	 * QString representation:
	 *
	 * @code
	 *  flag_map TestUtil::map
	 *  {
	 *      CONSTANT(EINVAL),
	 *      CONSTANT(EAGAIN),
	 *      CONSTANT(EBUSY)
	 *  };
	 * @endcode
	 *
	 * You may access the mapper like every std::unordered_map.
	 *
	 * @note The operator[] may throw a std::out_of_range exception. It
	 * is a safe way to map using the get() method instead.
	 */
	template<class T> class ConstantEnumerator : public std::unordered_map<T, QString>
	{
	public:
		/**
		 * This alias is a short cut to the containing key/value pairs.
		 */
		using ConstantMapPair          = std::pair<const T, QString>;

		/**
		 * This alias is a short cut to the containing iterator.
		 */
		using ConstantMapIterator      = typename std::unordered_map<T, QString>::iterator;

		/**
		 * This alias is a short cut to the containing const iterator.
		 */
		using ConstantMapConstIterator = typename std::unordered_map<T, QString>::const_iterator;

		/**
		 * This constructor makes it possible to initialize this mapper
		 * with a static initializer list.
		 *
		 * @param list The initializier list which may be provided by
		 * the compiler.
		 */
		explicit ConstantEnumerator(
			const std::initializer_list<ConstantMapPair> & list) :
			std::unordered_map<T, QString>(list)
		{
		}

		/**
		 * This method provides a safe way to represent a symbolic value
		 * as a QString. If the key is not present in the map its
		 * hexadecimal representation is returned instead.
		 *
		 * @param key The symbolic value.
		 * @return The QString representation of the symbolic value.
		 */
		inline QString get(const T key) const
		{
			ConstantMapConstIterator it = std::unordered_map<T, QString>::find(key);

			return it != std::unordered_map<T, QString>::end() ?
				it->second :
				QString::asprintf("0x%02X", (unsigned)key);
		}

		/**
		 * This method is a reverse lookup to find a key from its value. The
		 * return value is an const_iterator to a ConstantMapPair type. If this
		 * iterator is equal to end() no key was found. Otherwise you can
		 * select the key by using
		 *
		 * @code
		 * auto it = mapper.findKey("WHAT_EVER");
		 *
		 * if (it != mapper.end())
		 * {
		 *      auto key = it->first;
		 * }
		 * @endcode
		 *
		 * @param value The value from which the key should be reverse lookedup.
		 * @return A ConstantMapPair const_iterator which may contain the key
		 * when the const_iterator is not equal to the end() iterator.
		 */
		inline ConstantMapConstIterator findKey(const QString & value) const
		{
			ConstantMapConstIterator it;

			for (it = std::unordered_map<T, QString>::cbegin();
				it != std::unordered_map<T, QString>::cend();
				++it)
			{
				if (it->second == value)
				{
					break;
				}
			}
			return it;
		}
	};
}

#endif
