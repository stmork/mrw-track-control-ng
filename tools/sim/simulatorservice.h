//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef SIMULATORSERVICE_H
#define SIMULATORSERVICE_H

#include <can/mrwbusservice.h>
#include <model/modelrepository.h>
#include <model/device.h>

class SimulatorService : public mrw::can::MrwBusService
{
	Q_OBJECT

	mrw::model::ModelRailway * model = nullptr;

public:
	SimulatorService() = delete;

	explicit SimulatorService(
			mrw::model::ModelRepository & repo,
			QObject   *                   parent    = nullptr);
	virtual ~SimulatorService() = default;

	void info();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;

private:
	void    broadcast(const mrw::can::MrwMessage & message);
	void    append(mrw::can::MrwMessage & response, uint8_t size);
	void    device(const mrw::can::MrwMessage & message);
	void    setSwitchState(mrw::model::Device * device, const mrw::can::SwitchState switch_state);
	uint8_t getSwitchState(mrw::model::Device * device);
	uint8_t occupation(mrw::model::Device * device);
};

#endif
