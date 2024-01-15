//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <util/method.h>
#include <ctrl/controllerregistry.h>
#include <ctrl/basecontroller.h>

using namespace mrw::util;
using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

ControllerRegistry::ControllerRegistry() : QObject(nullptr)
{
	__METHOD__;
}

ControllerRegistry::~ControllerRegistry()
{
	__METHOD__;

	qInfo("  Shutting down controller registry.");
	Q_ASSERT(registry.empty());
}

void ControllerRegistry::registerController(
	Device        *        device,
	ControllerRegistrand * ctrl)
{
	registry.emplace(device, ctrl);
}

void ControllerRegistry::unregisterController(Device * device)
{
	registry.erase(device);
}

ControllerRegistrand * ControllerRegistry::find(Device * device) const
{
	if (device == nullptr)
	{
		return nullptr;
	}

	auto it = registry.find(device);
	return it != registry.end() ? it->second : nullptr;
}

void ControllerRegistry::registerService(MrwBusService * service)
{
	Q_ASSERT(can_service == nullptr);
	Q_ASSERT(service != nullptr);

	can_service = service;
}

MrwBusService * ControllerRegistry::can()
{
	Q_ASSERT(instance().can_service != nullptr);

	return instance().can_service;
}
