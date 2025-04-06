//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef SIMULATORSERVICE_H
#define SIMULATORSERVICE_H

#include <type_traits>

#include <QLoggingCategory>

#include <can/mrwbusservice.h>
#include <model/modelrepository.h>
#include <model/device.h>

/**
 * This service class simulates the behaviour of a modelrailway.
 */
class SimulatorService : public mrw::can::MrwBusService
{
	Q_OBJECT

private:
	QLoggingCategory           log;

	/** Interrupts per second. */
	static constexpr double    SLICE_COUNT = 16000000.0 / (1024 * 256);

	/** Time between timer interrupts in ms. */
	static constexpr double    SLICE       = 1000.0 / SLICE_COUNT;

	mrw::model::ModelRailway * model = nullptr;
	unsigned                   device_count = 0;

public:
	explicit SimulatorService(
		mrw::model::ModelRepository & repo,
		QObject           *           parent    = nullptr);
	SimulatorService() = delete;
	virtual ~SimulatorService() = default;

	void info();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;

private:
	void    broadcast(const mrw::can::MrwMessage & message);
	void    append(mrw::can::MrwMessage & response, uint8_t size);
	void    controller(
		const mrw::model::Controller * controller,
		const mrw::can::MrwMessage  &  message);
	void    device(const mrw::can::MrwMessage & message);
	bool    setSwitchState(mrw::model::Device * device, const mrw::can::SwitchState switch_state);
	std::underlying_type_t<mrw::can::SwitchState> getSwitchState(mrw::model::Device * device);
	uint8_t occupation(mrw::model::Device * device);
	void    bootSequence(const mrw::can::ControllerId id);

	static bool isFormSignal(const mrw::model::Device * device);
};

#endif
