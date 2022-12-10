//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef SIMULATORSERVICE_H
#define SIMULATORSERVICE_H

#include <can/mrwbusservice.h>
#include <model/modelrailway.h>
#include <model/device.h>

class SimulatorService : public mrw::can::MrwBusService
{
	Q_OBJECT

	mrw::model::ModelRailway * model = nullptr;

public:
	SimulatorService() = delete;

	explicit SimulatorService(
			const char * filename,
			const char * interface = "can0",
			const char * plugin    = "socketcan",
			QObject   *  parent    = nullptr);
	virtual ~SimulatorService();

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
