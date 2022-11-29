//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRWMESSAGEDISPATCHER_H
#define MRWMESSAGEDISPATCHER_H

#include <can/mrwbusservice.h>
#include <model/modelrailway.h>
#include <statecharts/OperatingMode.h>

class MrwMessageDispatcher : public mrw::can::MrwBusService
{
	Q_OBJECT

private:
	mrw::statechart::OperatingMode   statechart;
	mrw::model::ModelRailway    *    model   = nullptr;

public:
	MrwMessageDispatcher() = delete;
	explicit MrwMessageDispatcher(
		mrw::model::ModelRailway  *  model_railway,
		const char         *         interface = "can0",
		const char         *         plugin    = "socketcan",
		QObject           *          parent = nullptr);

	virtual ~MrwMessageDispatcher();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;
};

#endif
