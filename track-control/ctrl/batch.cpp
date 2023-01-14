//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>
#include <QDebug>

#include <ctrl/batch.h>
#include <ctrl/controllerregistrand.h>

using namespace mrw::ctrl;

/*************************************************************************
**                                                                      **
**       Transaction/Batch handling                                     **
**                                                                      **
*************************************************************************/

void Batch::reset()
{
	qDebug("======================= Transaction left %zu elements.", transaction.size());
	transaction.clear();
}

void Batch::increase(ControllerRegistrand * element)
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

void Batch::decrease(ControllerRegistrand * element)
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

bool Batch::contains(ControllerRegistrand * ctrl)
{
	return transaction.find(ctrl) != transaction.end();
}

void Batch::tryComplete()
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

bool Batch::isCompleted()
{
	return transaction.size() == 0;
}

void Batch::dump()
{
	for (ControllerRegistrand * registrand : transaction)
	{
		qDebug() << *registrand;
	}
}
