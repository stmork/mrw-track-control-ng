//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/batch.h>
#include <util/batchparticipant.h>
#include <util/log.h>

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

		qCWarning(log).noquote() << "Transaction participant not resetted:" << participant->name();

		// Since a transaction is also open in BatchParticipant it will
		// also call this->remove()
		participant->setBatch(nullptr);
	}
}

void Batch::reset()
{
	qCDebug(log, "======================= Transaction (ID=%u) left %zu elements.",
		id, transaction.size());

	auto it = transaction.begin();
	while (it != transaction.end())
	{
		BatchParticipant * participant = *it++;

		participant->setBatch(nullptr);
	}
	transaction.clear();
}

bool Batch::increase(BatchParticipant * element) noexcept
{
	if (transaction.find(element) == transaction.end())
	{
		transaction.emplace(element);
		qCDebug(log, "Transaction (ID=%u) increased to %zu element(s). Added: %s",
			id, transaction.size(),  element->name().toLatin1().constData());
		return true;
	}
	else
	{
		qCWarning(log).noquote() << "Transaction already contains element" << element->name();
	}
	return false;
}

bool Batch::decrease(BatchParticipant * element) noexcept
{
	const size_t count = transaction.erase(element);

	if (count == 1)
	{
		qCDebug(log, "Transaction (ID=%u) decreased to %zu element(s). Removed: %s",
			id, transaction.size(), element->name().toLatin1().constData());

		if (isCompleted())
		{
			qCDebug(log, "======================= Transaction (ID=%u) completed.",
				id);
			emit completed();
		}
		return true;
	}
	else
	{
		qCWarning(log, "Transaction (ID=%u) does not contain element %s",
			id,  element->name().toLatin1().constData());
	}
	return false;
}

bool Batch::contains(BatchParticipant * ctrl) const noexcept
{
	return transaction.find(ctrl) != transaction.end();
}

void Batch::tryComplete()
{
	QCoreApplication::instance()->processEvents();
	if (isCompleted())
	{
		qCDebug(log, "======================= Transaction (ID=%u) completed (was empty).",
			id);
		emit completed();
	}
	else
	{
		qCDebug(log, "======================= Transaction (ID=%u) contains %zu elements.",
			id, transaction.size());
	}
}

bool Batch::isCompleted() const noexcept
{
	return transaction.size() == 0;
}

void Batch::dump() const
{
	qCDebug(log, "======================= Transaction (ID=%u) contains %zu elements.",
		id, transaction.size());
	for (BatchParticipant * participant : transaction)
	{
		qCDebug(log).noquote() << participant->name();
	}
}

void Batch::remove(BatchParticipant * element) noexcept
{
	transaction.erase(element);
}
