//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_STRING_H
#define MRW_UTIL_STRING_H

#include <ostream>

#include <QString>
#include <QDebug>

namespace mrw::util
{
	/**
	 * This class is a unique way to create a std::string from any binary
	 * data representation. Only implement the pure virtual method toString().
	 */
	class String
	{
	public:
		/**
		 * This method returns the contents of this instance in human readable
		 * form.
		 *
		 * @return The human readable form of this instance.
		 */
		virtual QString toString() const = 0;

		/**
		 * This is a convenience cast operator to std::string.
		 *
		 * @see toString()
		 */
		inline operator QString() const
		{
			return toString();
		}
	};

	/**
	 * This shift operator writes the contents of a String instance as human
	 * readable text to the given output stream.
	 *
	 * @param os The output stream to write to.
	 * @param instance The instance containing the binary data.
	 * @return The given output stream for convenience.
	 *
	 * @see toString()
	 */
	inline std::ostream & operator<<(std::ostream & os, const String & instance)
	{
		os << instance.toString().toStdString();

		return os;
	}

	/**
	 * This shift operator writes the contents of a String instance as human
	 * readable text to the given QDebug instance.
	 *
	 * @param os The output stream to write to.
	 * @param instance The instance containing the binary data.
	 * @return The given output stream for convenience.
	 *
	 * @see toString()
	 */
	inline QDebug operator<<(QDebug debug, const String & instance)
	{
		QDebugStateSaver saver(debug);

		debug.nospace().noquote() << instance.toString();

		return debug;
	}
	/**
	 * This add operator allows convenient concatenation of a const char
	 * pointer string with a std::string. So you can use a Stringify instance
	 * without calling the String::toString() method:
	 * @code
	String some_data;

	mrw::util::debug("Using: " + some_data);
	@endcode
	 *
	 * @param left The C style const char pointer string.
	 * @param right The Stringify based data.
	 * @return a std::string instance of the concatenated string.
	 */
	inline QString operator+(const QString & left, const String & right)
	{
		return left + right.toString();
	}
}

#endif
