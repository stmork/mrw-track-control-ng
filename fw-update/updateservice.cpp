//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <regex>

#include <QCoreApplication>
#include <QFile>

#include <statecharts/timerservice.h>

#include "updateservice.h"

using namespace mrw::can;
using namespace mrw::statechart;

UpdateService::UpdateService(
	const QString & interface,
	const QString & plugin,
	QObject    *    parent) :
	MrwBusService(interface, plugin, parent),
	statechart(nullptr)
{
	read("/lib/firmware/mrw/mrw-firmware-m32.hex");

	statechart.setOperationCallback(this);
	statechart.setTimerService(&TimerService::instance());

	if (buffer != nullptr)
	{
		statechart.enter();
	}
	else
	{
		QCoreApplication::quit();
	}
}

UpdateService::~UpdateService()
{
	statechart.exit();

	if (buffer != nullptr)
	{
		free(buffer);
	}
}

const std::regex  hexline_regex(R"(^:([\da-fA-F]{2})([\da-fA-F]{4})([\da-fA-F]{2})(([\da-fA-F]{2})+))");

void UpdateService::read(const char * filename)
{
	QFile file(filename);
	const bool success = file.open(QIODevice::ReadOnly);

	if (success)
	{
		QTextStream in(&file);
		unsigned    count, type;
		unsigned    i, idx;

		while (!in.atEnd())
		{
			const std::string line = in.readLine().toStdString();

			// Adresse extrahieren
			if (sscanf(line.c_str(), ":%02x%04x%02x", &count, &address, &type) == 3)
			{
				uint8_t * bytes = nullptr;
				unsigned  line_checksum = type + count + (address >> 8) + (address & 0xff);

				switch (type)
				{
				case 0: //data
					size  = address + count;
					bytes = (uint8_t *)realloc(buffer, size + 1);
					if (bytes == nullptr)
					{
						free(buffer);
						throw std::bad_alloc();
					}
					buffer = bytes;
					bytes  = &buffer[address];
					for (i = 0, idx = 9; i <= count; i++)
					{
						unsigned byte = 0;

						sscanf(&line[idx], "%02x", &byte);
						bytes[i] = byte & 0xff;
						line_checksum += byte;
						idx += 2;
					}
					line_checksum &= 0xff;
					if (line_checksum != 0)
					{
						qWarning("Checksum error at address 0x%04x: %02x != %02x in file\n",
							address, line_checksum, bytes[count]);
					}
					break;

				case 1: // EOF
					break;
				}
			}
		}
		file.close();
	}
}

void UpdateService::ping()
{
	MrwMessage message(PING);

	write(message);
}

void UpdateService::reset()
{
	MrwMessage message(RESET);

	write(message);
}

void UpdateService::flashRequest(const uint8_t hid)
{
	MrwMessage message(FLASH_REQ);

	message.append(hid & 0xff);
	message.append(SIGNATURE_BYTE_1);
	message.append(SIGNATURE_BYTE_2);
	message.append(SIGNATURE_BYTE_3);

	write(message);
}

void UpdateService::flashRequest()
{
	flashRequest(DEFAULT_HARDWARE);
}

void UpdateService::flashData(const size_t bytes)
{
	MrwMessage message(FLASH_DATA);

	message.append(address & 0xff);
	message.append(address >> 8);
	message.append(address >> 16);

	for (size_t b = 0; b < bytes; b++)
	{
		const uint8_t data = buffer[address++];

		message.append(data);
		checksum += data;
	}

	write(message);
}

void UpdateService::flashCheck()
{
	MrwMessage message(FLASH_CHECK);

	message.append(address & 0xff);
	message.append(address >> 8);
	message.append(0);
	message.append(checksum & 0xff);

	write(message);
}

void UpdateService::quit()
{
	QCoreApplication::quit();
}

bool UpdateService::hasPages()
{
	return size >= SPM_PAGESIZE;
}

void UpdateService::flashCompletePage()
{
	for (size_t loop = 0; loop < SPM_PAGESIZE; loop += 4)
	{
		flashData(4);
	}
	qDebug("-----");
	size -= SPM_PAGESIZE;
}

void UpdateService::flashRestPage()
{
	for (size_t loop = 0; loop < size; loop += 2)
	{
		flashData(2);
	}
	qDebug("---");
	size = 0;
}
