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

TestFlankSwitch::TestFlankSwitch() : TestModel("Test-Flank")
{
}

void TestFlankSwitch::testFlankProtectionLeft()
{
	AbstractSwitch * s1 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 2, 0));
	AbstractSwitch * s2 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 2, 1));

	QVERIFY(s1 != nullptr);
	QVERIFY(s2 != nullptr);

	QVERIFY(!s1->isFlankProtection(s1));
	QVERIFY( s1->isFlankProtection(s2));
	QVERIFY( s2->isFlankProtection(s1));
	QVERIFY(!s2->isFlankProtection(s2));
}

void TestFlankSwitch::testFlankProtectionMid()
{
	AbstractSwitch * s3 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 6, 0));
	AbstractSwitch * s4 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 5, 0));
	AbstractSwitch * s5 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 7, 0));
	AbstractSwitch * s6 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 6, 2));
	AbstractSwitch * s7 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 5, 2));

	QVERIFY(s3 != nullptr);
	QVERIFY(s4 != nullptr);
	QVERIFY(s5 != nullptr);
	QVERIFY(s6 != nullptr);
	QVERIFY(s7 != nullptr);

	QVERIFY(!s3->isFlankProtection(s3));
	QVERIFY(!s3->isFlankProtection(s4));
	QVERIFY(!s3->isFlankProtection(s5));
	QVERIFY(!s3->isFlankProtection(s6));
	QVERIFY(!s3->isFlankProtection(s7));

	QVERIFY(!s4->isFlankProtection(s3));
	QVERIFY(!s4->isFlankProtection(s4));
	QVERIFY(!s4->isFlankProtection(s5));
	QVERIFY(!s4->isFlankProtection(s6));
	QVERIFY(!s4->isFlankProtection(s7));

	QVERIFY( s5->isFlankProtection(s3));
	QVERIFY( s5->isFlankProtection(s4));
	QVERIFY(!s5->isFlankProtection(s5));
	QVERIFY( s5->isFlankProtection(s6));
	QVERIFY( s5->isFlankProtection(s7));

	QVERIFY(!s6->isFlankProtection(s3));
	QVERIFY(!s6->isFlankProtection(s4));
	QVERIFY(!s6->isFlankProtection(s5));
	QVERIFY(!s6->isFlankProtection(s6));
	QVERIFY(!s6->isFlankProtection(s7));

	QVERIFY(!s7->isFlankProtection(s3));
	QVERIFY(!s7->isFlankProtection(s4));
	QVERIFY(!s7->isFlankProtection(s5));
	QVERIFY(!s7->isFlankProtection(s6));
	QVERIFY(!s7->isFlankProtection(s7));
}

void TestFlankSwitch::testFlankProtectionRight()
{
	AbstractSwitch * s8 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 10, 0));
	AbstractSwitch * s9 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 10, 1));

	QVERIFY(s8 != nullptr);
	QVERIFY(s9 != nullptr);

	QVERIFY(!s8->isFlankProtection(s8));
	QVERIFY( s8->isFlankProtection(s9));
	QVERIFY( s9->isFlankProtection(s8));
	QVERIFY(!s9->isFlankProtection(s9));
}
