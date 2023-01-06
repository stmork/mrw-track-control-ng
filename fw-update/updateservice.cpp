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
	MrwBusService(interface, plugin, parent, false),
	statechart(nullptr)
{
	buffer.reserve(65536);
	read("/lib/firmware/mrw/mrw-firmware-m32.hex");

	connect(
		this, &MrwBusService::connected,
		&statechart, &UpdateStatechart::connected,
		Qt::QueuedConnection);

	statechart.setOperationCallback(this);
	statechart.setTimerService(&TimerService::instance());
	statechart.enter();

	can_device->connectDevice();
}

UpdateService::~UpdateService()
{
	statechart.exit();
}

void UpdateService::read(const QString & filename)
{
	QFile file(filename);

	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		bool        loop = true;

		while (!in.atEnd() && loop)
		{
			const QString & input = in.readLine();
			const HexLine   hex_line(input);

			loop = hex_line;
			if (loop)
			{
				if (buffer.size() == hex_line.getAddress())
				{
					hex_line.append(buffer);
				}
				else
				{
					throw std::invalid_argument("Address out of range: " + hex_line.getAddress());
				}
			}
		}
		file.close();
	}
	else
	{
		throw std::invalid_argument("File not found: " + filename.toStdString());
	}
}

void UpdateService::process(const MrwMessage & message)
{
	MrwBusService::process(message);

	if (message.isResponse())
	{
		switch (message.command() & CMD_MASK)
		{
		case PING:
			controller_ids.insert(message.eid());
			break;

		case RESET:
			check(message, MSG_RESET_PENDING);
			break;

		case FLASH_REQ:
			check(message);
			break;

		case FLASH_CHECK:
			if (message.response() == MSG_CHECKSUM_ERROR)
			{
				statechart.failed();
			}
			else
			{
				check(message);
			}
			break;

		case GETVER:
			check(message);
			break;
		}
	}
}

void UpdateService::init()
{
	request_ids.clear();
	std::copy(
		controller_ids.begin(), controller_ids.end(),
		std::inserter(request_ids, request_ids.begin()));
}

bool UpdateService::check(const MrwMessage & message, const Response response)
{
	if (message.response() == response)
	{
		const ControllerId id = message.eid();

		request_ids.erase(id);
		if (request_ids.empty())
		{
			statechart.complete();
			return true;
		}
	}
	return false;
}

void UpdateService::ping()
{
	MrwMessage message(PING);

	write(message);
}

void UpdateService::boot()
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
	// Init values.
	rest     = buffer.size();
	address  = 0;
	checksum = 0;

	// Request flashing
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

void UpdateService::fail(sc::integer error_code)
{
	switch (error_code)
	{
	case 0:
		qInfo("No error occured but reached fail state?");
		break;

	case 1:
		qCritical("Timeout after calling PING!");
		break;

	case 2:
		qCritical("Timeout after calling RESET!");
		break;

	case 3:
		qCritical("Checksum error after flash!");
		break;

	case 4:
		qCritical("Timeout after checksum check!");
		break;

	case 5:
		qCritical("Timeout while booting!");
		break;

	case 6:
		qCritical("Retry exceeded requesting flash!");
		break;

	case 7:
		qCritical("Timeout connecting to CAN bus!");
		break;

	default:
		qCritical("Unknown error occured!");
		break;
	}

	QCoreApplication::exit(error_code);
}

bool UpdateService::hasController()
{
	return controller_ids.size() > 0;
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
