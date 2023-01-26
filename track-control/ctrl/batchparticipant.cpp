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
			batch->increase(this);
		}
		else
		{
			qWarning("inc: Batch already active!");
		}
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
		if (open_tx == batch)
		{
			batch->decrease(this);
		}
		else
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
