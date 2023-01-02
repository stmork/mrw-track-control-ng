//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef HEXLINE_H
#define HEXLINE_H

#include <cstdint>
#include <string>
#include <vector>

struct HexLine
{
	unsigned address;
	unsigned count;
	unsigned type;
	unsigned checksum = 0;
	bool     eof      = false;

	std::vector<uint8_t> bytes;

	HexLine(const std::string & line);

	void append(std::vector<uint8_t> & buffer) const;

	operator bool() const;
};

#endif
