//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

#include <memory>
#include <unordered_set>

#include <util/self.h>
#include <can/mrwbusservice.h>
#include <statecharts/timerservice.h>
#include <statecharts/ConfigStatechart.h>
#include <model/modelrepository.h>

/**
 * This class provides a service for configururing the CAN nodes.
 */
class ConfigurationService :
	public mrw::can::MrwBusService,
	public mrw::util::SelfPointer<mrw::statechart::ConfigStatechart::OperationCallback>,
	public mrw::statechart::ConfigStatechart::OperationCallback
{
	Q_OBJECT

private:
	mrw::statechart::QtStatechart<mrw::statechart::ConfigStatechart>   statechart;

	/** The model of this modelrailway. */
	mrw::model::ModelRailway         *         model = nullptr;

	/** The set of controller IDs. */
	std::unordered_set<mrw::can::ControllerId> controllers;

	/** Devices inside model. */
	size_t                                     device_count = 0;

	/** Devices configured. */
	size_t                                     config_count = 0;

public:
	explicit ConfigurationService(
		mrw::model::ModelRepository & repo,
		QObject           *           parent    = nullptr);
	ConfigurationService() = delete;
	virtual ~ConfigurationService();

	void info();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;

private:
	void sendConfig(
		const mrw::can::ControllerId              id,
		const std::vector<mrw::can::MrwMessage> & messages);

	sc::integer configure(sc::integer idx) override;
	bool hasMore(sc::integer idx) override;

	void booting() override;
	void quit() override;
	void fail() override;
};

#endif
