//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <util/method.h>
#include <util/globalbatch.h>

using namespace mrw::util;

GlobalBatch::GlobalBatch()
{
	__METHOD__;
}

GlobalBatch::~GlobalBatch()
{
	__METHOD__;

	Q_ASSERT(transaction.empty());

	qInfo("  Global transaction (ID=%u) closed.", id);
}
