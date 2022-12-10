//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <can/mrwmessage.h>
#include "model/modelrailway.h"
#include "model/device.h"

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::model;

using LockState = Device::LockState;

const ConstantEnumerator<LockState>  Device::lock_map
{
	{ LockState::FAIL,     "FAIL" },
	{ LockState::UNLOCKED, "UNLOCKED" },
	{ LockState::PENDING,  "PENDING" },
	{ LockState::LOCKED,   "LOCKED" }
};

Device::Device(
	ModelRailway     *    model_railway,
	const QDomElement  &  element) :
	unit_no(ModelRailway::value(element, "unit_no"))
{
	if (unit_no == 0)
	{
		model_railway->error(element.nodeName() + " has no unit number!");
	}
	model_railway->add(this);
}

MrwMessage Device::command(const Command command) const
{
	return MrwMessage(command, controller()->id(), unitNo());
}

QString Device::get(const mrw::model::Device::LockState & state)
{
	return lock_map.get(state);
}
