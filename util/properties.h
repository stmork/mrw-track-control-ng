//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_PROPERTIES_H
#define MRW_UTIL_PROPERTIES_H

#include <unordered_map>
#include <regex>

#include <QString>

namespace mrw::util
{
	/**
	 * This class simulates a Java style properties file. Basically it is
	 * an unordered map.
	 */
	class Properties : public std::unordered_map<std::string, std::string>
	{
		static const std::regex  regex;

	public:
		Properties() = default;
		/**
		 * This constructor reads in a Java style properties file.
		 *
		 * @param filename The properties file to read.
		 * @see read();
		 */
		explicit Properties(const QString & filename);

		/**
		 * This method reads in a Java style properties file.
		 *
		 * @param filename The properties file to read.
		 * @return True on success.
		 */
		bool read(const QString & filename);
	};
}

#endif // MRW_UTIL_PROPERTIES_H