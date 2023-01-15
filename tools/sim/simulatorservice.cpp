//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <model/section.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>

#include "simulatorservice.h"

using namespace mrw::can;
using namespace mrw::model;

SimulatorService::SimulatorService(
	ModelRepository & repo,
	QObject     *     parent) :
	MrwBusService(repo.interface(), repo.plugin(), parent)
{
	model = repo;
}

void SimulatorService::info()
{
	if (model != nullptr)
	{
		model->info();
	}
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
		else if (message.unitNo() != NO_UNITNO)
		{
			device(message);
		}
		else
		{
			const Controller * ctrl = model->controllerById(message.sid());
			controller(ctrl, message);
		}
	}
	else
	{
		// Ignore responses.
	}
}

void SimulatorService::broadcast(const MrwMessage & message)
{
	for (size_t c = 0; c < model->controllerCount(); c++)
	{
		const Controller * ctrl = model->controller(c);

		controller(ctrl, message);
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

void SimulatorService::controller(
	const Controller * controller,
	const MrwMessage & message)
{
	const Command cmd = message.command();

	MrwMessage   response(controller->id(), NO_UNITNO, cmd, MSG_OK);
	bool         answer = true;

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

	case FLASH_DATA:
	case FLASH_REQ:
		answer = false;
		break;

	case FLASH_CHECK:
		response.append(message[3]);
		break;

	default:
		// No additional payload needed.
		break;
	}

	if (answer)
	{
		write(response);
	}

	switch (cmd)
	{
	case RESET:
	case SET_ID:
	case CFGEND:
		response = MrwMessage(controller->id(), NO_UNITNO, cmd, MSG_RESET_PENDING);
		write(response);
		break;

	default:
		// No additional payload needed.
		break;
	}
}

void SimulatorService::device(const MrwMessage & message)
{
	const ControllerId id      = message.sid();
	const UnitNo       unit_no = message.unitNo();
	const Command      cmd     = message.command();
	Device      *      device  = model->deviceById(id, unit_no);
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
