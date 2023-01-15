//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

#include <can/mrwbusservice.h>
#include <model/modelrepository.h>

class ConfigurationService : public mrw::can::MrwBusService
{
	Q_OBJECT

	mrw::model::ModelRailway * model = nullptr;

public:
	explicit ConfigurationService(
		mrw::model::ModelRepository & repo,
		QObject           *           parent    = nullptr);
	virtual ~ConfigurationService() = default;

	void info();
	void configure();

private:
	void sendConfig(
		const mrw::can::ControllerId              id,
		const std::vector<mrw::can::MrwMessage> & messages);
};

#endif
