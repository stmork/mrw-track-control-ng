//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <exception>

#include <QString>

#include "hexline.h"

HexLine::HexLine(const std::string & line)
{
	bytes.reserve(64);

	// Adresse extrahieren
	if (sscanf(line.c_str(), ":%02x%04x%02x", &count, &address, &type) == 3)
	{
		unsigned i, idx;

		checksum = type + count + (address >> 8) + (address & 0xff);

		switch (type)
		{
		case 0: //data
			for (i = 0, idx = 9; i <= count; i++)
			{
				unsigned byte = 0;

				sscanf(&line[idx], "%02x", &byte);
				bytes.push_back(byte & 0xff);
				checksum += byte;
				idx += 2;
			}
			checksum &= 0xff;
			if (checksum != 0)
			{
				const std::string message = QString::asprintf(
							"Checksum error at address 0x%04x: %02x != %02x in file\n",
						 address, checksum, bytes[count]).toStdString();

				throw std::invalid_argument(message);
			}
			break;

		case 1: // EOF
			eof = true;
			break;
		}
	}
}

void HexLine::append(std::vector<uint8_t> & buffer) const
{
	std::copy(bytes.begin(), bytes.begin() + count, std::back_inserter(buffer));
}

HexLine::operator bool() const
{
	return !eof;
}
