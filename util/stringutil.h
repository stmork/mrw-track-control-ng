//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_STRINGUTIL_H
#define MRW_UTIL_STRINGUTIL_H

#include <ostream>

#include <QString>
#include <QDebug>

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#	define SKIP_EMPTY_PARTS Qt::SkipEmptyParts
#else
#	define SKIP_EMPTY_PARTS QString::SkipEmptyParts
#endif


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

		/**
		 * This method surrounds a string with a given console format command.
		 * After the given input string to format command is reset.
		 *
		 * @param code The format command to switch on.
		 * @param input The input string to surround with format commands.
		 * @return The formatted string.
		 */
		static inline QString format(
			const QString & code,
			const QString & input)
		{
			return code + input + ALL_OFF;
		}

		/**
		 * This method surrounds a string with a console format code to
		 * print the input string as bold. After the given input string to
		 * format command is reset.
		 *
		 * @param input The input string to surround with format commands.
		 * @return The formatted string.
		 */
		static inline QString Bold(const QString & input)
		{
			return format(BOLD_ON, input);
		}

		static const QString RED_ON;
		static const QString GREEN_ON;
		static const QString BLUE_ON;
		static const QString MAGENTA_ON;
		static const QString CYAN_ON;
		static const QString WHITE_ON;

		static const QString BOLD_ON;
		static const QString ITALIC_ON;
		static const QString UNDERLINE_ON;
		static const QString ALL_OFF;
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
	 * @param debug The output debug instance to write to.
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
	 * @param right The String based data.
	 * @return a QString instance of the concatenated string.
	 */
	inline QString operator+(const QString & left, const String & right)
	{
		return left + right.toString();
	}
}

#endif
