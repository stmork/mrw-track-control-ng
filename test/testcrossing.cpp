//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QTest>

#include "testcrossing.h"

using namespace mrw::test;
using namespace mrw::model;

TestCrossing::TestCrossing() : TestModelBase("Test-Crossing")
{
}

void TestCrossing::testSimple()
{
	QVERIFY(model->valid());
	QVERIFY(model->controller(0)->valid());
	QVERIFY(model->controller(1)->valid());
}
