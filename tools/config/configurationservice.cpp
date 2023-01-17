//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>
#include <QTimer>

#include <statecharts/timerservice.h>

#include "configurationservice.h"

using namespace mrw::can;
using namespace mrw::statechart;
using namespace mrw::model;

ConfigurationService::ConfigurationService(
	mrw::model::ModelRepository & repo,
	QObject           *           parent) :
	MrwBusService(repo.interface(), repo.plugin(), parent),
	statechart(nullptr)
{
	connect(
		this, &MrwBusService::connected,
		&statechart, &ConfigStatechart::connected,
		Qt::QueuedConnection);

	statechart.setTimerService(&TimerService::instance());
	statechart.setOperationCallback(this);

	Q_ASSERT(statechart.check());
	statechart.enter();

	model = repo;
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
		if ((message.command() == RESET) && (message.response() == MSG_BOOTED))
		{
			const size_t count = controllers.erase(message.eid());

			if ((count > 0) && (controllers.size() == 0))
			{
				statechart.completed();
			}
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

void ConfigurationService::configure(sc::integer idx)
{
	std::vector<MrwMessage> messages;
	const Controller    *   controller = model->controller(idx);
	const ControllerId      id = controller->id();


	qDebug("---------------------- %u", id);
	controllers.insert(id);
	controller->configure(messages);

#if 0
	for (const MrwMessage & msg : messages)
	{
		qDebug().noquote() << msg;
	}
#else
	sendConfig(id, messages);
#endif
	statechart.sent();
}

bool ConfigurationService::hasMore(sc::integer idx)
{
	return idx < (int)model->controllerCount();
}

void ConfigurationService::booting()
{
	qInfo("Configuration completed and booting.");
}

void ConfigurationService::quit()
{
	qInfo("Booted.");
	QCoreApplication::quit();
}

void ConfigurationService::fail()
{
	qCritical("Configuration timeout!");
	QCoreApplication::exit(EXIT_FAILURE);
}
