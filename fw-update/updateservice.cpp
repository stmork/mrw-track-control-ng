//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <regex>

#include <QCoreApplication>
#include <QFile>

#include <statecharts/timerservice.h>

#include "updateservice.h"
#include "hexline.h"

using namespace mrw::can;
using namespace mrw::statechart;

UpdateService::UpdateService(
	const QString & interface,
	const QString & plugin,
	QObject    *    parent) :
	MrwBusService(interface, plugin, parent),
	statechart(nullptr)
{
	buffer.reserve(65536);
	read("/lib/firmware/mrw/mrw-firmware-m32.hex");

	statechart.setOperationCallback(this);
	statechart.setTimerService(&TimerService::instance());
	statechart.enter();
}

UpdateService::~UpdateService()
{
	statechart.exit();
}

void UpdateService::read(const QString & filename)
{
	QFile  file(filename);
	bool   loop = true;

	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);

		while (!in.atEnd())
		{
			const std::string & line = in.readLine().toStdString();
			const HexLine       hex_line(line);

			loop = hex_line;
			if (loop)
			{
				if (buffer.size() == hex_line.address)
				{
					hex_line.append(buffer);
				}
				else
				{
					throw std::invalid_argument("Address out of range: " + hex_line.address);
				}
			}
		}
		file.close();
		rest = buffer.size();
	}
	else
	{
		throw std::invalid_argument("File not found: " + filename.toStdString());
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
	return rest >= SPM_PAGESIZE;
}

void UpdateService::flashCompletePage()
{
	for (size_t loop = 0; loop < SPM_PAGESIZE; loop += 4)
	{
		flashData(4);
	}
	qDebug("-----");
	rest -= SPM_PAGESIZE;
}

void UpdateService::flashRestPage()
{
	for (size_t loop = 0; loop < rest; loop += 2)
	{
		flashData(2);
	}
	qDebug("---");
	rest = 0;
}
