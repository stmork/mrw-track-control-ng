//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTest>

#include <can/commands.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>

#include "testflankswitch.h"

using namespace mrw::can;
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
	AbstractSwitch * s5 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 7, 1));
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

bool contains(const std::vector<RegularSwitch *> & switches, RegularSwitch * part)
{
	return std::find(switches.begin(), switches.end(), part) != switches.end();
}

void TestFlankSwitch::testFlankProtectionDcs()
{
	RegularSwitch   *   s3 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 6, 0));
	RegularSwitch   *   s4 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 5, 0));
	DoubleCrossSwitch * s5 = dynamic_cast<DoubleCrossSwitch *>(model->assemblyPart(0, 7, 1));
	RegularSwitch   *   s6 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 6, 2));
	RegularSwitch   *   s7 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 5, 2));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s3 != nullptr);
	QVERIFY(s4 != nullptr);
	QVERIFY(s5 != nullptr);
	QVERIFY(s6 != nullptr);
	QVERIFY(s7 != nullptr);

	flank_switches.clear();
	s5->setState(DoubleCrossSwitch::State::AC);
	QCOMPARE(s5->flank(flank_switches, true), 2u);
	QVERIFY( contains(flank_switches, s3));
	QVERIFY(!contains(flank_switches, s4));
	QVERIFY( contains(flank_switches, s6));
	QVERIFY(!contains(flank_switches, s7));
	QCOMPARE(s3->switchState(), SWITCH_STATE_RIGHT);
	QCOMPARE(s6->switchState(), SWITCH_STATE_LEFT);

	flank_switches.clear();
	s5->setState(DoubleCrossSwitch::State::AD);
	QCOMPARE(s5->flank(flank_switches, true), 2u);
	QVERIFY( contains(flank_switches, s3));
	QVERIFY(!contains(flank_switches, s4));
	QVERIFY(!contains(flank_switches, s6));
	QVERIFY( contains(flank_switches, s7));
	QCOMPARE(s3->switchState(), SWITCH_STATE_RIGHT);
	QCOMPARE(s7->switchState(), SWITCH_STATE_RIGHT);

	flank_switches.clear();
	s5->setState(DoubleCrossSwitch::State::BC);
	QCOMPARE(s5->flank(flank_switches, true), 2u);
	QVERIFY(!contains(flank_switches, s3));
	QVERIFY( contains(flank_switches, s4));
	QVERIFY( contains(flank_switches, s6));
	QVERIFY(!contains(flank_switches, s7));
	QCOMPARE(s4->switchState(), SWITCH_STATE_LEFT);
	QCOMPARE(s6->switchState(), SWITCH_STATE_LEFT);

	flank_switches.clear();
	s5->setState(DoubleCrossSwitch::State::BD);
	QCOMPARE(s5->flank(flank_switches, true), 2u);
	QVERIFY(!contains(flank_switches, s3));
	QVERIFY( contains(flank_switches, s4));
	QVERIFY(!contains(flank_switches, s6));
	QVERIFY( contains(flank_switches, s7));
	QCOMPARE(s4->switchState(), SWITCH_STATE_LEFT);
	QCOMPARE(s7->switchState(), SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionRsLeft()
{
	RegularSwitch * s1 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 2, 0));
	RegularSwitch * s2 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 2, 1));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s1 != nullptr);
	QVERIFY(s2 != nullptr);

	flank_switches.clear();
	s1->setState(RegularSwitch::State::AB);
	QCOMPARE(s1->flank(flank_switches, true), 0u);
	QVERIFY(!contains(flank_switches, s2));
	QCOMPARE(s1->switchState(), SWITCH_STATE_LEFT);
	QCOMPARE(s2->switchState(), SWITCH_STATE_LEFT);

	flank_switches.clear();
	s1->setState(RegularSwitch::State::AC);
	QCOMPARE(s1->flank(flank_switches, true), 1u);
	QVERIFY(contains(flank_switches, s2));
	QCOMPARE(s1->switchState(), SWITCH_STATE_RIGHT);
	QCOMPARE(s2->switchState(), SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionRsRight()
{
	RegularSwitch * s8 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 10, 0));
	RegularSwitch * s9 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 10, 1));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s8 != nullptr);
	QVERIFY(s9 != nullptr);

	flank_switches.clear();
	s8->setState(RegularSwitch::State::AC);
	QCOMPARE(s8->flank(flank_switches, true), 0u);
	QVERIFY(!contains(flank_switches, s9));
	QCOMPARE(s8->switchState(), SWITCH_STATE_RIGHT);
	QCOMPARE(s9->switchState(), SWITCH_STATE_RIGHT);

	flank_switches.clear();
	s8->setState(RegularSwitch::State::AB);
	QCOMPARE(s8->flank(flank_switches, true), 1u);
	QVERIFY(contains(flank_switches, s9));
	QCOMPARE(s8->switchState(), SWITCH_STATE_LEFT);
	QCOMPARE(s9->switchState(), SWITCH_STATE_LEFT);
}
