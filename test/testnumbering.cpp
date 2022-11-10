//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>

#include <model/railswitch.h>

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
	QVERIFY(!model->controller(0)->valid());
}

void TestNumbering::testMissingModule()
{
	QVERIFY(!model->section(0, 0)->valid());
}

void TestNumbering::testDirectionFail()
{
	QVERIFY( model->assemblyPart(0, 0, 0)->valid());
	QVERIFY(!model->assemblyPart(0, 0, 1)->valid());
	QVERIFY(!model->assemblyPart(0, 0, 2)->valid());
	QVERIFY(!model->assemblyPart(0, 0, 3)->valid());
}

void TestNumbering::testEmptyFail()
{
	QVERIFY(!model->assemblyPart(0, 1, 0)->valid());
	QVERIFY(!model->assemblyPart(0, 1, 1)->valid());
	QVERIFY(!model->assemblyPart(0, 1, 2)->valid());
}

void TestNumbering::testToManyLights()
{
	QVERIFY(!model->controller(0)->connection(0)->valid());
	QVERIFY(!model->module(0, 0)->valid());
}

void TestNumbering::testToManyInductors()
{
	QVERIFY(!model->module(0, 1)->valid());
}

void TestNumbering::testPorts()
{
	QCOMPARE(model->module(1, 0)->ports(), 1);
	QCOMPARE(model->module(1, 1)->ports(), 1);
	QCOMPARE(model->module(1, 2)->ports(), 1);
	QCOMPARE(model->module(2, 0)->ports(), 2);
}
