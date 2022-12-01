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

class MrwMessageDispatcher :
	public mrw::can::MrwBusService,
	public mrw::statechart::OperatingMode::OperationCallback
{
	Q_OBJECT

private:
	mrw::statechart::OperatingMode   statechart;
	mrw::model::ModelRailway    *    model   = nullptr;

public:
	MrwMessageDispatcher() = delete;
	explicit MrwMessageDispatcher(
		mrw::model::ModelRailway  *  model_railway,
		const QString        &       interface = "can0",
		const QString        &       plugin    = "socketcan",
		QObject           *          parent    = nullptr);

	virtual ~MrwMessageDispatcher();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;
	virtual void connectBus() override;
	virtual void reset() override;
};

#endif
