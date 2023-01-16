//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>
#include <QTimer>

#include "configurationservice.h"

using namespace mrw::can;
using namespace mrw::model;

ConfigurationService::ConfigurationService(
	mrw::model::ModelRepository & repo,
	QObject           *           parent) :
	MrwBusService(repo.interface(), repo.plugin(), parent)
{
	model = repo;
}

void ConfigurationService::info()
{
	if (model != nullptr)
	{
		model->info();
	}
}

void ConfigurationService::configure()
{
	for (size_t c = 0; c < model->controllerCount(); c++)
	{
		std::vector<MrwMessage> messages;
		const Controller    *   controller = model->controller(c);
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
	}
	QTimer::singleShot(
		model->controllerCount() * 200 + 3500,
		this, &ConfigurationService::timeout);
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
				completed();
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

void ConfigurationService::completed()
{
	qInfo("Configuration completed.");
	QCoreApplication::quit();
}

void ConfigurationService::timeout()
{
	qCritical("Configuration timeout!");
	QCoreApplication::exit(EXIT_FAILURE);
}
