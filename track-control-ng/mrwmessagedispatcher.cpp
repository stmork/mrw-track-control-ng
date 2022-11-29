//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QDebug>

#include <ctrl/controllerregistry.h>

#include "mrwmessagedispatcher.h"

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

MrwMessageDispatcher::MrwMessageDispatcher(
	mrw::model::ModelRailway  *  model_railway,
	const char         *         interface,
	const char         *         plugin,
	QObject           *          parent) :
	MrwBusService(interface, plugin, parent),
	model(model_railway)
{
}

void MrwMessageDispatcher::process(const MrwMessage & message)
{
	const ControllerId dst     = message.sid();

	if (message.isResponse() && (dst == CAN_GATEWAY_ID))
	{
		const UnitNo       unit_no = message.unitNo();

		Device * device = model->deviceByUnitNo(unit_no);

		if (device != nullptr)
		{
			ControllerRegistrand * controller = ControllerRegistry::instance().find(device);

			if (controller != nullptr)
			{
				controller->process(message);
			}
		}
	}
	else
	{
		qDebug().noquote() << message;
	}
}
