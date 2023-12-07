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

class HexLine
{
	unsigned address;
	unsigned count;
	unsigned type;
	unsigned checksum = 0;
	bool     eof      = false;

	std::vector<uint8_t> bytes;

public:
	explicit HexLine(const QString & input);

	void append(std::vector<uint8_t> & buffer) const;

	operator bool() const;

	unsigned getAddress() const;
};

#endif
