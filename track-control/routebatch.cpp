//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "routebatch.h"

RouteBatch::RouteBatch(QObject * parent) : QObject(parent)
{
}

RouteBatch::~RouteBatch()
{
	qDebug("======================= Disable transaction (ID=%u) deleted.", id);
}
