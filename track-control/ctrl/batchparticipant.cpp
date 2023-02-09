//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ctrl/batch.h>
#include <ctrl/batchparticipant.h>
#include <ctrl/controllerregistry.h>

using namespace mrw::ctrl;

BatchParticipant::BatchParticipant()
{
	base_tx = &ControllerRegistry::instance();
}

void BatchParticipant::increase()
{
	if (base_tx != nullptr)
	{
		if (open_tx == nullptr)
		{
			open_tx = base_tx;
		}
		else if (open_tx != base_tx)
		{
			qWarning("inc: Another batch is already active!");
		}
		base_tx->increase(this);
	}
	else
	{
		qWarning("inc: No batch active!");
	}
}

void BatchParticipant::decrease()
{
	if (base_tx != nullptr)
	{
		base_tx->decrease(this);
		if (open_tx != base_tx)
		{
			qWarning().noquote() << "dec: Batch different!" << name();
		}
	}
	else
	{
		qWarning("dec: No batch active!");
	}
}

Batch * BatchParticipant::batch() const
{
	return base_tx;
}

void BatchParticipant::setBatch(Batch * new_batch)
{
	if (new_batch != nullptr)
	{
		qDebug() << "+setBatch()" << name();
	}
	else
	{
		qDebug() << "-setBatch()" << name();
		new_batch = &ControllerRegistry::instance();
	}

	if (base_tx != new_batch)
	{
		base_tx = new_batch;
		open_tx = nullptr;
	}
}
