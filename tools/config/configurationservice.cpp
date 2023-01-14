//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

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
		Controller       *      controller = model->controller(c);

		qDebug("---------------------- %u", controller->id());
		controller->configure(messages);

		for (const MrwMessage & msg : messages)
		{
			qDebug().noquote() << msg;
		}
	}
}
