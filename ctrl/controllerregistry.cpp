//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/controllerregistry.h>

using namespace mrw::can;
using namespace mrw::model;
using namespace mrw::ctrl;

mrw::ctrl::ControllerRegistry::ControllerRegistry() : QObject(nullptr)
{
}

ControllerRegistry::~ControllerRegistry()
{
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

ControllerRegistrand * ControllerRegistry::find(Device * device)
{
	return registry[device];
}

void ControllerRegistry::registerService(mrw::can::MrwBusService * service)
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
