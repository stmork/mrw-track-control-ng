//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>

#include <statecharts/timerservice.h>

#include "updateservice.h"

using namespace mrw::can;
using namespace mrw::statechart;

UpdateService::UpdateService(
		const QString & interface,
		const QString & plugin,
		QObject *       parent) :
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
		quit();
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

void UpdateService::read(const char * filename)
{
	FILE *    file = fopen(filename, "r");
	char      line[128];
	unsigned  i, idx;

	if (file != NULL)
	{
		unsigned count, type;

		while (fgets(line, sizeof(line), file) == line)
		{
			// Adresse extrahieren
			if (sscanf(line, ":%02x%04x%02x", &count, &address, &type) == 3)
			{
				uint8_t * bytes = nullptr;
				unsigned  line_checksum = type + count + (address >> 8) + (address & 0xff);

				switch (type)
				{
				case 0: //data
					size   = address + count;
					buffer = (unsigned char *)realloc(buffer, size + 1);
					bytes  = &buffer[address];
					for (i = 0, idx = 9; i <= count; i++)
					{
						int byte = 0;

						sscanf(&line[idx], "%02x", &byte);
						bytes[i] = byte & 0xff;
						line_checksum += byte;
						idx += 2;
					}
					line_checksum &= 0xff;
					if (line_checksum != 0)
					{
						fprintf(stderr, "Checksum error at address 0x%04x: %02x != %02x in file\n",
							address, line_checksum, bytes[count]);
					}
					break;

				case 1: // EOF
					break;
				}
#if 0
				printf("%2u %04x %d - %4x\n", count, address, type, checksum);
				if (bytes != NULL)
				{
					printf(" %02x", bytes[count]);
				}
				printf("\n");
#endif
			}
		}
		fclose(file);
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
