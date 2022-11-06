//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>

#include "testnumbering.h"
#include "testdef.h"

using namespace mrw::test;
using namespace mrw::model;

TestNumbering::TestNumbering() : TestModelBase("Test-Numbering")
{
}

void TestNumbering::testInvalid()
{
	QVERIFY(!model->isValid());
}
