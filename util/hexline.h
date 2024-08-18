//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_HEXLINE_H
#define MRW_UTIL_HEXLINE_H

#include <cstdint>
#include <string>
#include <vector>

namespace mrw::util
{
	/**
	 * This class parses one single hex coded line into a byte array.
	 */
	class HexLine
	{
		unsigned address;
		unsigned count;
		unsigned type;
		unsigned checksum = 0;
		bool     eof      = false;

		std::vector<uint8_t> bytes;

	public:
		HexLine() = delete;
		explicit HexLine(const QString & input);

		/**
		 * This cast operator returns false if the parsed line contained an
		 * end of file marker.
		 *
		 * @return true If this hex line contained normal data.
		 */
		operator bool() const noexcept;

		/**
		 * This method returns the 16-bit flash memory address of the hexline.
		 *
		 * @return The 16-bit flash memory address.
		 */
		unsigned getAddress() const noexcept;

		void append(std::vector<uint8_t> & buffer) const;
	};
}

#endif
