//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <util/method.h>
#include <ctrl/globalbatch.h>

using namespace mrw::ctrl;

GlobalBatch::GlobalBatch()
{
	__METHOD__;
}

GlobalBatch::~GlobalBatch()
{
	__METHOD__;

	if (transaction.empty())
	{
		qInfo("  Global transaction (ID=%u) closed.", id);
	}
	else
	{
		qWarning("======================= Transaction (ID=%u) left %zu elements.",
			id, transaction.size());
	}
}
