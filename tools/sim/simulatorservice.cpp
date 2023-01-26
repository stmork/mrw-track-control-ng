//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTimer>

#include <model/section.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>
#include <model/formsignal.h>

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

	case CFGBGN:
		device_count = 0;
		break;

	case CFGEND:
		response.append(device_count);
		break;

	case FLASH_DATA:
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
		[[fallthrough]];
	case FLASH_CHECK:
		bootSequence(controller->id());
		break;

	default:
		// No additional payload needed.
		break;
	}
}

void SimulatorService::device(const MrwMessage & message)
{
	const ControllerId     id      = message.sid();
	const UnitNo           unit_no = message.unitNo();
	const Command          cmd     = message.command();
	Device        *        device  = model->deviceById(id, unit_no);
	Response               code    = device != nullptr ? MSG_OK : MSG_UNIT_NOT_FOUND;
	std::vector<uint8_t>   appendix;
	std::function<void()>  late_ok = [this, id, unit_no, cmd]()
	{
		const MrwMessage msg(id, unit_no, cmd, MSG_OK);

		write(msg);
	};

	switch (cmd)
	{
	case SETLFT:
		code = MSG_QUEUED;
		setSwitchState(device, SWITCH_STATE_LEFT);
		QTimer::singleShot(75, late_ok);
		break;

	case SETRGT:
		code = MSG_QUEUED;
		setSwitchState(device, SWITCH_STATE_RIGHT);
		QTimer::singleShot(75, late_ok);
		break;

	case GETDIR:
		appendix.push_back(getSwitchState(device));
		break;

	case GETRBS:
		appendix.push_back(occupation(device));
		break;

	case SETSGN:
		if (isFormSignal(device))
		{
			code = MSG_QUEUED;
			QTimer::singleShot(750, late_ok);
		}
		break;

	case CFGMF2:
	case CFGMF3:
	case CFGPF2:
	case CFGPF3:
	case CFGLGT:
	case CFGRAI:
	case CFGSWN:
	case CFGSWO:
	case CFGML2:
	case CFGML3:
	case CFGML4:
	case CFGPL2:
	case CFGPL3:
	case CFGSL2:
		device_count++;
		break;

	default:
		// No additional action needed.
		break;
	}

	MrwMessage response(id, unit_no, cmd, code);
	for (const uint8_t b : appendix)
	{
		response.append(b);
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

void SimulatorService::bootSequence(const ControllerId id)
{
	MrwMessage response(id, NO_UNITNO, GETVER, MSG_OK);
	response.append(3);
	response.append(1);
	response.append(0x23);
	response.append(0x45);
	write(response);

	response = MrwMessage(id, NO_UNITNO, RESET, MSG_BOOTED);
	response.append(0);
	write(response);
}

bool SimulatorService::isFormSignal(const Device * device)
{
	return dynamic_cast<const FormSignal *>(device) != nullptr;
}
