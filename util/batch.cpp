//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>
#include <QDebug>

#include <util/batch.h>
#include <util/batchparticipant.h>

using namespace mrw::util;

std::atomic_uint32_t   Batch::counter;

/*************************************************************************
**                                                                      **
**       Transaction/Batch handling                                     **
**                                                                      **
*************************************************************************/

Batch::Batch() : id(++counter)
{
}

Batch::~Batch()
{
	while (!transaction.empty())
	{
		BatchParticipant * participant = *transaction.begin();

		qWarning().noquote() << "Transaction participant not resetted:" << participant->name();

		// Since a transaction is also open in BatchParticipant it will
		// also call this->remove()
		participant->setBatch(nullptr);
	}
}

void Batch::reset()
{
	qDebug("======================= Transaction (ID=%u) left %zu elements.",
		id, transaction.size());

	for (BatchParticipant * participant : transaction)
	{
		participant->setBatch(nullptr);
	}
	transaction.clear();
}

bool Batch::increase(BatchParticipant * element)
{
	if (transaction.find(element) == transaction.end())
	{
		transaction.emplace(element);
		qDebug("Transaction (ID=%u) increased to %zu element(s). Added: %s",
			id, transaction.size(),  element->name().toLatin1().constData());
		return true;
	}
	else
	{
		qWarning().noquote() << "Transaction already contains element" << element->name();
	}
	return false;
}

bool Batch::decrease(BatchParticipant * element)
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
		return true;
	}
	else
	{
		qWarning("Transaction (ID=%u) does not contain element %s",
			id,  element->name().toLatin1().constData());
	}
	return false;
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
	qDebug("======================= Transaction (ID=%u) contains %zu elements.",
		id, transaction.size());
	for (BatchParticipant * participant : transaction)
	{
		qDebug() << participant->name();
	}
}

void Batch::remove(mrw::util::BatchParticipant * element)
{
	transaction.erase(element);
}
