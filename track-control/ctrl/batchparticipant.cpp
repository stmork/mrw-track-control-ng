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
	batch = &ControllerRegistry::instance();
}

void BatchParticipant::increase()
{
	if (batch != nullptr)
	{
		if (open_tx == nullptr)
		{
			open_tx = batch;
		}
		else
		{
			qWarning("inc: Batch already active!");
		}
		batch->increase(this);
	}
	else
	{
		qWarning("inc: No batch active!");
	}
}

void BatchParticipant::decrease()
{
	if (batch != nullptr)
	{
		batch->decrease(this);
		if (open_tx != batch)
		{
			qWarning("dec: Batch different!");
		}
		open_tx = nullptr;
	}
	else
	{
		qWarning("dec: No batch active!");
	}
}

void BatchParticipant::setBatch(Batch * new_batch)
{
	if (new_batch != nullptr)
	{
		batch = new_batch;
	}
	else
	{
		batch = &ControllerRegistry::instance();
	}
}
