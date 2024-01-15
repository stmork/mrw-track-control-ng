//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include "routebatch.h"

RouteBatch::RouteBatch(
	const bool  has_signal,
	const bool  is_auto_unblock,
	QObject  *  parent) :
	QObject(parent),
	limit(has_signal ? 2 : 1),
	auto_unblock(is_auto_unblock)
{
}

RouteBatch::~RouteBatch()
{
	qDebug("======================= Disable transaction (ID=%u) deleted.", id);
}

void RouteBatch::completed()
{
	count++;
	if (count >= limit)
	{
		if (auto_unblock)
		{
			emit unlock();
		}
		else
		{
			emit tryUnblock();
		}
		deleteLater();
	}
}
