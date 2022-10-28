//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <unistd.h>

#include "util/method.h"
#include "testutil.h"

using namespace mrw::test;

TestUtil::TestUtil(QObject * parent) : QObject(parent)
{
}

void TestUtil::testMethod()
{
	__METHOD__;

	usleep(123456);
}
