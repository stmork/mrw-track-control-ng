//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <model/section.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>

#include "simulatorservice.h"

using namespace mrw::can;
using namespace mrw::model;

SimulatorService::SimulatorService(
	const char * filename,
	const char * interface,
	const char * plugin,
	QObject   *  parent) : MrwBusService(interface, plugin, parent)
{
	model = new ModelRailway(filename);
}

SimulatorService::~SimulatorService()
{
	delete model;
}

void SimulatorService::process(const MrwMessage & message)
{
	qInfo() << message;

	if (!message.isResponse())
	{
		// Process request!
		if (message.sid() == CAN_BROADCAST_ID)
		{
			broadcast(message);
		}
		else
		{
			device(message);

		}
	}
	else
	{
		// Ignore responses.
	}
}

void SimulatorService::broadcast(const MrwMessage & message)
{
	const Command cmd = message.command();

	for (size_t c = 0; c < model->controllerCount(); c++)
	{
		Controller * controller = model->controller(c);
		MrwMessage   response(controller->id(), NO_UNITNO, cmd, MSG_OK);

		switch (cmd)
		{
		case GETVER:
			response.append(3);
			response.append(1);
			response.append(0x23);
			response.append(0x45);
			break;

		case QRYBUF:
			append(response, 1);
			break;

		case QRYERR:
			append(response, 3);
			break;

		default:
			// No additional payload needed.
			break;
		}

		write(response);
	}
}

void SimulatorService::append(MrwMessage & response, uint8_t size)
{
	response.append(size);
	for (uint8_t b = 0; b < size; b++)
	{
		response.append(0);
	}
}

void SimulatorService::device(const MrwMessage & message)
{
	const ControllerId id      = message.sid();
	const UnitNo       unit_no = message.unitNo();
	const Command      cmd     = message.command();
	Device      *      device  = model->deviceByUnitNo(unit_no);
	MrwMessage         response(id, unit_no, cmd, device != nullptr ? MSG_OK : MSG_UNIT_NOT_FOUND);

	switch (cmd)
	{
	case SETLFT:
		setSwitchState(device, SWITCH_STATE_LEFT);
		break;

	case SETRGT:
		setSwitchState(device, SWITCH_STATE_RIGHT);
		break;

	case GETDIR:
		response.append(getSwitchState(device));
		break;

	case GETRBS:
		response.append(occupation(device));
		break;

	default:
		// No additional action needed.
		break;
	}

	write(response);
}

uint8_t SimulatorService::getSwitchState(Device * device)
{
	AbstractSwitch * rs = dynamic_cast<AbstractSwitch *>(device);

	return rs->switchState();
}

void SimulatorService::setSwitchState(Device * device, const SwitchState switch_state)
{
	RegularSwitch   *   rs  = dynamic_cast<RegularSwitch *>(device);
	DoubleCrossSwitch * dcs = dynamic_cast<DoubleCrossSwitch *>(device);

	if (rs != nullptr)
	{
		rs->setState(static_cast<RegularSwitch::State>(switch_state));
	}
	if (dcs != nullptr)
	{
		dcs->setState(static_cast<DoubleCrossSwitch::State>(switch_state));
	}
}

uint8_t SimulatorService::occupation(Device * device)
{
	Section * section = dynamic_cast<Section *>(device);

	return section->occupation();
}
