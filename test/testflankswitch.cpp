//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>

#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>

#include "testflankswitch.h"

using namespace mrw::test;
using namespace mrw::model;

TestFlankSwitch::TestFlankSwitch() : TestModel("Test-Railway")
{
}

void TestFlankSwitch::testFlankProtection()
{
	AbstractSwitch * s1 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 2, 0));
	AbstractSwitch * s2 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 2, 1));
	AbstractSwitch * s3 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(2, 2, 0));
	AbstractSwitch * s4 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(2, 2, 1));

	QVERIFY(s1 != nullptr);
	QVERIFY(s2 != nullptr);
	QVERIFY(s3 != nullptr);
	QVERIFY(s4 != nullptr);

	QVERIFY(!s1->isFlankProtection(s1));
	QVERIFY(s1->isFlankProtection(s2));
	QVERIFY(!s1->isFlankProtection(s3));
	QVERIFY(!s1->isFlankProtection(s4));

	QVERIFY(s2->isFlankProtection(s1));
	QVERIFY(!s2->isFlankProtection(s2));
	QVERIFY(!s2->isFlankProtection(s3));
	QVERIFY(!s2->isFlankProtection(s4));

	QVERIFY(!s3->isFlankProtection(s1));
	QVERIFY(!s3->isFlankProtection(s2));
	QVERIFY(!s3->isFlankProtection(s3));
	QVERIFY(s3->isFlankProtection(s4));

	QVERIFY(!s4->isFlankProtection(s1));
	QVERIFY(!s4->isFlankProtection(s2));
	QVERIFY(!s4->isFlankProtection(s3));
	QVERIFY(!s4->isFlankProtection(s4));
}
