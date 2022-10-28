//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>
#include <QDebug>

#include "testutil.h"

using namespace mrw::test;

int main(int argc, char * argv[])
{
	TestUtil test_util;

	return QTest::qExec(&test_util, argc, argv);
}
