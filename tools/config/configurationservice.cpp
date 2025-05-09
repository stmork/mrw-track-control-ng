//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <cassert>

#include <QCoreApplication>
#include <QTimer>

#include <statecharts/timerservice.h>

#include "configurationservice.h"

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::statechart;
using namespace mrw::model;

ConfigurationService::ConfigurationService(
	ModelRepository & repo,
	QObject     *     parent) :
	MrwBusService(repo.interface(), repo.plugin(), parent, false),
	log("mrw.tools.config")
{
	std::vector<Device *> devices;

	model = repo;
	if (model != nullptr)
	{
		model->parts<Device>(devices);
		device_count = devices.size();
	}

	connect(
		this, &MrwBusService::connected,
		&statechart, &ConfigStatechart::connected,
		Qt::QueuedConnection);

	statechart.setTimerService(TimerService::instance());
	statechart.setOperationCallback(*this);

	Q_ASSERT(statechart.check());
	statechart.enter();

	can_device->connectDevice();
}

ConfigurationService::~ConfigurationService()
{
	statechart.exit();
}

void ConfigurationService::info()
{
	if (model != nullptr)
	{
		model->info();
	}
}

void ConfigurationService::process(const MrwMessage & message)
{
	if (message.isResponse())
	{
		const Command  cmd      = message.command();
		const Response response = message.response();

		switch (cmd)
		{
		case RESET:
			if (response == Response::MSG_BOOTED)
			{
				const size_t count = controllers.erase(message.eid());

				qCDebug(log, "---------------------- (%zu controllers left)",
					controllers.size());
				if ((count > 0) && (controllers.size() == 0))
				{
					statechart.completed();
				}
			}
			break;

		case CFGEND:
			if ((response == Response::MSG_OK) && (message.size() >= 1))
			{
				const size_t count = message[0];

				config_count += count;
			}
			break;

		default:
			break;
		}
	}
}

void ConfigurationService::sendConfig(
	const ControllerId              id,
	const std::vector<MrwMessage> & messages)
{
	const MrwMessage cfg_begin(CFGBGN, id);
	const MrwMessage cfg_end(  CFGEND, id);

	write(cfg_begin);
	for (const MrwMessage & msg : messages)
	{
		write(msg);
	}
	write(cfg_end);
}

sc::integer ConfigurationService::configure(sc::integer idx)
{
	std::vector<MrwMessage> messages;
	const Controller    *   controller = model->controller(idx);
	const ControllerId      id = controller->id();

	controllers.insert(id);
	qCDebug(log, "---------------------- %u (%zu unprocessed controllers)",
		id, controllers.size());
	controller->configure(messages);

#if 0
	for (const MrwMessage & msg : messages)
	{
		qCDebug(log).noquote() << msg;
	}
#else
	sendConfig(id, messages);
#endif

	return messages.size();
}

bool ConfigurationService::hasMore(sc::integer idx)
{
	return idx < (int)model->controllerCount();
}

void ConfigurationService::booting()
{
	qCInfo(log, "Configuration completed and booting.");
	if (controllers.size() == 0)
	{
		statechart.completed();
	}
}

void ConfigurationService::quit()
{
	qCInfo(log, "Configured devices:   %3zu", config_count);
	qCInfo(log, "Max devices per node: %3d", statechart.getMax());
	qCInfo(log, "Assembly parts:       %3zu", device_count);
	qCInfo(log, "Model railway facility ready.");
	QCoreApplication::quit();
}

void ConfigurationService::fail()
{
	qCCritical(log, "Configuration timeout!");
	QCoreApplication::exit(EXIT_FAILURE);
}
