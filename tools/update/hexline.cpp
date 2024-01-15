//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <exception>

#include <QString>

#include "hexline.h"

HexLine::HexLine(const QString & input)
{
	const std::string line = input.toStdString();

	// Adresse extrahieren
	if (sscanf(line.c_str(), ":%02x%04x%02x", &count, &address, &type) == 3)
	{
		checksum = type + count + (address >> 8) + (address & 0xff);

		const QByteArray  & array = QByteArray::fromHex(input.toLatin1().mid(9));

		if (array.length() != ((input.length() - 9) / 2))
		{
			throw std::invalid_argument("Input not in hex format!");
		}
		if (array.length() != (int)(count + 1))
		{
			throw std::invalid_argument("Count specifier invalid!");
		}
		bytes.reserve(64);

		switch (type)
		{
		case 0: //data
			for (int c : array)
			{
				unsigned byte = (unsigned)c & 0xff;

				bytes.push_back(byte);
				checksum += byte;
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

unsigned HexLine::getAddress() const
{
	return address;
}

HexLine::operator bool() const
{
	return !eof;
}
