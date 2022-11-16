//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>

#include <model/rail.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>
#include <model/formsignal.h>
#include <model/lightsignal.h>

#include "testnumbering.h"
#include "testdef.h"

using namespace mrw::test;
using namespace mrw::model;

TestNumbering::TestNumbering() : TestModelBase("Test-Numbering")
{
}

void TestNumbering::testInvalid()
{
	QVERIFY(!model->valid());
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

void TestNumbering::testPairSignalSection1()
{
	LightSignal * lr1 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 0, 4));
	LightSignal * lr2 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 0, 5));
	LightSignal * lr3 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 0, 6));

	QVERIFY(lr1 == nullptr);
	QVERIFY(lr2 == nullptr);
	QVERIFY(lr3 == nullptr);
}

void TestNumbering::testPairSignalSection2()
{
	LightSignal * lr1 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 1, 3));
	LightSignal * lr2 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 1, 4));
	LightSignal * lr3 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 1, 5));
	LightSignal * lr4 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 1, 6));
	LightSignal * lr5 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 1, 7));
	LightSignal * lr6 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 1, 8));

	QVERIFY(lr1 != nullptr);
	QVERIFY(lr2 != nullptr);
	QVERIFY(lr3 != nullptr);
	QVERIFY(lr4 != nullptr);
	QVERIFY(lr5 == nullptr);
	QVERIFY(lr6 == nullptr);

	QCOMPARE(lr1->pair(), lr4);
	QCOMPARE(lr2->pair(), lr4);
	QCOMPARE(lr3->pair(), lr4);
	QCOMPARE(lr4->pair(), nullptr);
}

void TestNumbering::testPairSignalSection3()
{
	LightSignal * ll1 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 2, 0));
	LightSignal * ll2 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 2, 1));
	LightSignal * lr1 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 2, 2));
	LightSignal * lr2 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 2, 3));
	LightSignal * lr3 = dynamic_cast<LightSignal *>(model->assemblyPart(0, 2, 4));

	QVERIFY(ll1 != nullptr);
	QVERIFY(ll2 != nullptr);
	QVERIFY(lr1 != nullptr);
	QVERIFY(lr2 != nullptr);
	QVERIFY(lr3 != nullptr);

	QCOMPARE(ll1->pair(), ll2);
	QCOMPARE(ll2->pair(), nullptr);
	QCOMPARE(lr1->pair(), lr2);
	QCOMPARE(lr2->pair(), nullptr);
	QCOMPARE(lr3->pair(), nullptr);
}

void TestNumbering::testSignalCount()
{
	std::vector<FormSignal *>  form_signals;
	std::vector<LightSignal *> light_signals;

	model->parts<FormSignal>(form_signals);
	model->parts<LightSignal>(light_signals);

	QCOMPARE(light_signals.size(), 9);
	QCOMPARE(form_signals.size(), 5);
}

void TestNumbering::testRailCount()
{
	std::vector<Rail *>              rails;
	std::vector<RegularSwitch *>     switches;
	std::vector<DoubleCrossSwitch *> doubles;

	model->parts<Rail>(rails);
	model->parts<RegularSwitch>(switches);
	model->parts<DoubleCrossSwitch>(doubles);

	QCOMPARE(rails.size(), 3);
	QCOMPARE(switches.size(), 3);
	QCOMPARE(doubles.size(), 1);
}
