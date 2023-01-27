//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>
#include <QDebug>

#include <ctrl/batch.h>
#include <ctrl/batchparticipant.h>

using namespace mrw::ctrl;

std::atomic_uint32_t   Batch::counter;

/*************************************************************************
**                                                                      **
**       Transaction/Batch handling                                     **
**                                                                      **
*************************************************************************/

Batch::Batch() : id(++counter)
{
}

void Batch::reset()
{
	qDebug("======================= Transaction (ID=%u) left %zu elements.",
		id, transaction.size());
	transaction.clear();
}

void Batch::increase(BatchParticipant * element)
{
	if (transaction.find(element) == transaction.end())
	{
		transaction.emplace(element);
		qDebug("Transaction (ID=%u) increased to %zu element(s). Added: %s",
			id, transaction.size(),  element->name().toLatin1().constData());
	}
	else
	{
		qWarning().noquote() << "Transaction already contains element" << element->name();
	}
}

void Batch::decrease(BatchParticipant * element)
{
	const size_t count = transaction.erase(element);

	if (count == 1)
	{
		qDebug("Transaction (ID=%u) decreased to %zu element(s). Removed: %s",
			id, transaction.size(), element->name().toLatin1().constData());

		if (isCompleted())
		{
			qDebug("======================= Transaction (ID=%u) completed.",
				id);
			emit completed();
		}
	}
	else
	{
		qWarning("Transaction (ID=%u) does not contain element %s",
			id,  element->name().toLatin1().constData());
	}
}

bool Batch::contains(BatchParticipant * ctrl)
{
	return transaction.find(ctrl) != transaction.end();
}

void Batch::tryComplete()
{
	QCoreApplication::instance()->processEvents();
	if (isCompleted())
	{
		qDebug("======================= Transaction (ID=%u) completed (was empty).",
			id);
		emit completed();
	}
	else
	{
		qDebug("======================= Transaction (ID=%u) contains %zu elements.",
			id, transaction.size());
	}
}

bool Batch::isCompleted()
{
	return transaction.size() == 0;
}

void Batch::dump()
{
	for (BatchParticipant * registrand : transaction)
	{
		qDebug() << registrand->name();
	}
}
