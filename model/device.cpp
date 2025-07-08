//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

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
	CONSTANT(LockState::FAIL),
	CONSTANT(LockState::UNLOCKED),
	CONSTANT(LockState::PENDING),
	CONSTANT(LockState::LOCKED)
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

UnitNo Device::unitNo() const noexcept
{
	return unit_no;
}

Device::LockState Device::lock() const noexcept
{
	return lock_state;
}

void Device::setLock(const LockState input) noexcept
{
	if (input == LockState::FAIL)
	{
		qCCritical(log).noquote() << String::red(" Locking to FAIL!") << name();
	}
	if (lock_state != input)
	{
		if ((input == LockState::UNLOCKED) && !isUnlockable())
		{
			qCWarning(log).noquote() << String::red("Not unlockable!") << name();
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

QString Device::get(const Device::LockState & state) noexcept
{
	return lock_map.get(state);
}

bool Device::hasController(const Device * device) noexcept
{
	return (device != nullptr) && (device->controller() != nullptr);
}
