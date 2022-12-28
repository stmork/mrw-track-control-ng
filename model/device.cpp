//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QDebug>

#include <util/stringutil.h>
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
}

UnitNo Device::unitNo() const
{
	return unit_no;
}

Device::LockState Device::lock() const
{
	return lock_state;
}

void Device::setLock(const Device::LockState input)
{
	if (input == LockState::FAIL)
	{
		qCritical().noquote() << String::red(" Locking to FAIL!") << name();
	}
	if (lock_state != input)
	{
		if ((input == LockState::UNLOCKED) && !isUnlockable())
		{
			qWarning().noquote() << String::red("Not unlockable!") << name();
		}
		else
		{
			lock_state = input;
		}
	}
}

MrwMessage Device::command(const Command command) const
{
	if (controller() == nullptr)
	{
		throw std::invalid_argument("No controller specified!");
	}
	return MrwMessage(command, controller()->id(), unitNo());
}

QString Device::get(const Device::LockState & state)
{
	return lock_map.get(state);
}
