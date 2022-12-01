//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ctrl/controllerregistry.h>
#include <util/method.h>

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
	qInfo("Shutting down controller registry.");
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

void ControllerRegistry::increase(ControllerRegistrand * ctrl)
{
	if (transaction.find(ctrl) == transaction.end())
	{
		transaction.emplace(ctrl);
		qDebug("Transaction increased to %zu elements.", transaction.size());
	}
	else
	{
		qWarning("Transaction already contains element %p.", ctrl);
	}
}

void ControllerRegistry::decrease(ControllerRegistrand * ctrl)
{
	if (transaction.erase(ctrl) == 1)
	{
		qDebug("Transaction decreased to %zu elements.", transaction.size());

		if (transaction.size() == 0)
		{
			emit completed();
		}
	}
	else
	{
		qWarning("Transaction does not contain element %p.", ctrl);
	}
}

void ControllerRegistry::reset()
{
	qDebug("Transaction left %zu elements.", transaction.size());
	transaction.clear();
}
