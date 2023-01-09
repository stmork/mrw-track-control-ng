//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>

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

	qInfo(" Shutting down controller registry.");
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

/*************************************************************************
**                                                                      **
**       Transaction/Batch handling                                     **
**                                                                      **
*************************************************************************/

void ControllerRegistry::reset()
{
	qDebug("======================= Transaction left %zu elements.", transaction.size());
	transaction.clear();
}

void ControllerRegistry::increase(ControllerRegistrand * element)
{
	if (transaction.find(element) == transaction.end())
	{
		transaction.emplace(element);
		qDebug().noquote() << "Transaction increased to" << transaction.size() << "element(s). Added: " << *element;
	}
	else
	{
		qWarning().noquote() << "Transaction already contains element" << *element;
	}
}

void ControllerRegistry::decrease(ControllerRegistrand * element)
{
	const size_t count = transaction.erase(element);

	if (count == 1)
	{
		qDebug().noquote() << "Transaction decreased to" << transaction.size() << "element(s). Removed: " << *element;

		if (isCompleted())
		{
			qDebug("======================= Transaction completed.");
			emit completed();
		}
	}
	else
	{
		qWarning().noquote() << "Transaction does not contain element" << *element;
	}
}

bool ControllerRegistry::contains(ControllerRegistrand * ctrl)
{
	return transaction.find(ctrl) != transaction.end();
}

void ControllerRegistry::tryComplete()
{
	QCoreApplication::instance()->processEvents();
	if (isCompleted())
	{
		qDebug("======================= Transaction completed (was empty).");
		emit completed();
	}
	else
	{
		qWarning("======================= Transaction left %zu elements.", transaction.size());
	}
}

bool ControllerRegistry::isCompleted()
{
	return transaction.size() == 0;
}

void ControllerRegistry::dump()
{
	for (ControllerRegistrand * registrand : transaction)
	{
		qDebug() << *registrand;
	}
}
