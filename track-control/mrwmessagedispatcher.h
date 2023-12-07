//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRWMESSAGEDISPATCHER_H
#define MRWMESSAGEDISPATCHER_H

#include <statecharts/OperatingModeStatechart.h>
#include <can/mrwbusservice.h>
#include <model/modelrailway.h>

class MrwMessageDispatcher :
	public mrw::can::MrwBusService,
	public mrw::statechart::OperatingModeStatechart::Can::OperationCallback
{
	Q_OBJECT

private:
	mrw::model::ModelRailway    *    model   = nullptr;

public:
	MrwMessageDispatcher() = delete;
	explicit MrwMessageDispatcher(
		mrw::model::ModelRailway  *  model_railway,
		const QString        &       interface = "can0",
		const QString        &       plugin    = "socketcan",
		QObject           *          parent    = nullptr);

	virtual ~MrwMessageDispatcher();

signals:
	void brightness(unsigned value);

public slots:
	void emergencyStop();

protected:
	virtual void process(const mrw::can::MrwMessage & message) override;
	virtual bool filter(const mrw::can::MrwMessage & message);
	virtual void connectBus() override;
	virtual bool isConnected() override;
};

#endif
