//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QDebug>

#include <util/batch.h>
#include <util/batchparticipant.h>
#include <util/globalbatch.h>

using namespace mrw::util;

BatchParticipant::BatchParticipant()
{
	base_tx = &GlobalBatch::instance();
}

BatchParticipant::~BatchParticipant()
{
	if (open_tx != nullptr)
	{
		qWarning("Forced decrease while destroying");
		open_tx->remove(this);
	}
}

bool BatchParticipant::increase() noexcept
{
	Q_ASSERT(base_tx != nullptr);

	if (open_tx == nullptr)
	{
		open_tx = base_tx;
	}
	else
	{
		Q_ASSERT(open_tx == base_tx);
	}

	return base_tx->increase(this);
}

bool BatchParticipant::decrease() noexcept
{
	Q_ASSERT(base_tx != nullptr);

	if (open_tx != base_tx)
	{
		qWarning().noquote() << "dec: Batch different!" << name();
	}
	open_tx = nullptr;
	return base_tx->decrease(this);
}

Batch * BatchParticipant::batch() const noexcept
{
	return base_tx;
}

void BatchParticipant::setBatch(Batch * new_batch) noexcept
{
	if (new_batch != nullptr)
	{
		qDebug() << "+setBatch()" << name();
	}
	else
	{
		qDebug() << "-setBatch()" << name();
		new_batch = &GlobalBatch::instance();
	}

	if (base_tx != new_batch)
	{
		base_tx = new_batch;
		if (open_tx != nullptr)
		{
			open_tx->remove(this);
		}
	}
	open_tx = nullptr;
}
