//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>

#include "resetservice.h"

using namespace mrw::can;

ResetService::ResetService(
	const QString & interface,
	const QString & plugin,
	QObject    *    parent) : MrwBusService(interface, plugin, parent, false)
{
	QObject::connect(this, &MrwBusService::connected, [&]()
	{
		const MrwMessage message(RESET);

		write(message);
	});

	can_device->connectDevice();
}

void ResetService::process(const MrwMessage & message)
{
	qDebug().noquote() << message;

	if (message.isResponse())
	{
		const Command      cmd      = message.command();
		const Response     response = message.response();
		const ControllerId id       = message.eid();

		switch (cmd)
		{
		case RESET:
			switch (response)
			{
			case MSG_OK:
			case MSG_RESET_PENDING:
				insert(id);
				break;

			case MSG_BOOTED:
				remove(id);
				return;

			default:
				// Intentionally do nothing
				break;
			}
			break;

		case GETVER:
			if (response == MSG_OK)
			{
				remove(id);
			}
			break;

		default:
			break;
		}
	}
}

void ResetService::insert(const ControllerId id)
{
	controllers.emplace(id);
}

void ResetService::remove(const ControllerId id)
{
	auto it = controllers.find(id);

	if (it != controllers.end())
	{
		controllers.erase(it);
		if (controllers.empty())
		{
			QCoreApplication::quit();
		}
	}
}
