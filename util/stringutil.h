//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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

/**
 * The mrw::util namespace contains some classes to make live easier. ;-)
 *
 * It contains string preparation classes, signal handlers, logging support
 * and some other little stuff like random number generation or a simple
 * constant to string mapper.
 */
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
		operator QString() const;

		/**
		 * This method surrounds a string with a given console format command.
		 * After the given input string to format command is reset.
		 *
		 * @note The final ALL_OFF formatting code deactivates all previous
		 * codes so you have to prepare the strings as your needs by multiple
		 * calls to this method.
		 *
		 * @code
		 * const QString & result =
			String::format(RED_ON + ITALIC_ON, "red italic") +
			String::format(ITALIC_ON, " only italic");
		 * @endcode
		 *
		 * @param code The format command to switch on. You can concatinate any
		 * of the console formatting code given below.
		 * @param input The input string to surround with format commands.
		 * @return The formatted string.
		 */
		static QString format(
			const QString & code,
			const QString & input);

		/**
		 * This method surrounds a string with a console format code to
		 * print the input string as bold. After the given input string to
		 * format command is reset.
		 *
		 * @param input The input string to surround with format commands.
		 * @return The formatted string.
		 */
		static QString bold(const QString & input);

		/**
		 * This method surrounds a string with a console format code to
		 * print the input string in red. After the given input string to
		 * format command is reset.
		 *
		 * @param input The input string to surround with format commands.
		 * @return The formatted string.
		 */
		static QString red(const QString & input);

		static const QString RED_ON;     ///< Turns console output into red.
		static const QString GREEN_ON;   ///< Turns console output into green.
		static const QString BLUE_ON;    ///< Turns console output into blue.
		static const QString MAGENTA_ON; ///< Turns console output into magenta.
		static const QString CYAN_ON;    ///< Turns console output into cyan.
		static const QString WHITE_ON;   ///< Turns console output into white.

		static const QString BOLD_ON;       ///< Turns console output into bold.
		static const QString ITALIC_ON;     ///< Turns console output into italic.
		static const QString UNDERLINE_ON;  ///< Turns on underlined console output.

		/** This turns off all of the previously activated console formatting. */
		static const QString ALL_OFF;
	};
}

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
std::ostream & operator<<(std::ostream & os, const mrw::util::String & instance);

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
QDebug operator<<(QDebug debug, const mrw::util::String & instance);

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
QString operator+(const QString & left, const mrw::util::String & right);

#endif
