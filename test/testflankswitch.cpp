//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QTest>

#include <can/commands.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>
#include <model/rail.h>

#include "testflankswitch.h"
#include "testroute.h"

using namespace mrw::can;
using namespace mrw::test;
using namespace mrw::model;

using LockState = Device::LockState;

TestFlankSwitch::TestFlankSwitch() : TestModel("Test-Flank")
{
}

void TestFlankSwitch::init()
{
	clear();
}

void TestFlankSwitch::testFlankProtectionLeft()
{
	AbstractSwitch * s1 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 1, 1));
	AbstractSwitch * s2 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 1, 2));

	QVERIFY(s1 != nullptr);
	QVERIFY(s2 != nullptr);

	QVERIFY(!s1->isFlankProtection(s1));
	QVERIFY( s1->isFlankProtection(s2));
	QVERIFY( s2->isFlankProtection(s1));
	QVERIFY(!s2->isFlankProtection(s2));
}

void TestFlankSwitch::testFlankProtectionCross()
{
	AbstractSwitch * s3 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 4, 1));
	AbstractSwitch * s4 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 2, 0));
	AbstractSwitch * s5 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 7, 1));
	AbstractSwitch * s6 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 9, 0));
	AbstractSwitch * s7 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 8, 0));

	QVERIFY(s3 != nullptr);
	QVERIFY(s4 != nullptr);
	QVERIFY(s5 != nullptr);
	QVERIFY(s6 != nullptr);
	QVERIFY(s7 != nullptr);

	QVERIFY(!s3->isFlankProtection(nullptr));
	QVERIFY(!s3->isFlankProtection(s3));
	QVERIFY(!s3->isFlankProtection(s4));
	QVERIFY(!s3->isFlankProtection(s5));
	QVERIFY(!s3->isFlankProtection(s6));
	QVERIFY(!s3->isFlankProtection(s7));

	QVERIFY(!s4->isFlankProtection(nullptr));
	QVERIFY(!s4->isFlankProtection(s3));
	QVERIFY(!s4->isFlankProtection(s4));
	QVERIFY(!s4->isFlankProtection(s5));
	QVERIFY(!s4->isFlankProtection(s6));
	QVERIFY(!s4->isFlankProtection(s7));

	QVERIFY(!s5->isFlankProtection(nullptr));
	QVERIFY( s5->isFlankProtection(s3));
	QVERIFY( s5->isFlankProtection(s4));
	QVERIFY(!s5->isFlankProtection(s5));
	QVERIFY( s5->isFlankProtection(s6));
	QVERIFY( s5->isFlankProtection(s7));

	QVERIFY(!s6->isFlankProtection(nullptr));
	QVERIFY(!s6->isFlankProtection(s3));
	QVERIFY(!s6->isFlankProtection(s4));
	QVERIFY(!s6->isFlankProtection(s5));
	QVERIFY(!s6->isFlankProtection(s6));
	QVERIFY(!s6->isFlankProtection(s7));

	QVERIFY(!s7->isFlankProtection(nullptr));
	QVERIFY(!s7->isFlankProtection(s3));
	QVERIFY(!s7->isFlankProtection(s4));
	QVERIFY(!s7->isFlankProtection(s5));
	QVERIFY(!s7->isFlankProtection(s6));
	QVERIFY(!s7->isFlankProtection(s7));
}

void TestFlankSwitch::testFlankProtectionRight()
{
	AbstractSwitch * s8 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 10, 0));
	AbstractSwitch * s9 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(0, 11, 0));

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
	RegularSwitch   *   s3 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 4, 1));
	RegularSwitch   *   s4 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 2, 0));
	DoubleCrossSwitch * s5 = dynamic_cast<DoubleCrossSwitch *>(model->assemblyPart(0, 7, 1));
	RegularSwitch   *   s6 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 9, 0));
	RegularSwitch   *   s7 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 8, 0));
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
	QCOMPARE(s3->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s6->switchState(), SwitchState::SWITCH_STATE_LEFT);

	flank_switches.clear();
	s5->setState(DoubleCrossSwitch::State::AD);
	QCOMPARE(s5->flank(flank_switches, true), 2u);
	QVERIFY( contains(flank_switches, s3));
	QVERIFY(!contains(flank_switches, s4));
	QVERIFY(!contains(flank_switches, s6));
	QVERIFY( contains(flank_switches, s7));
	QCOMPARE(s3->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s7->switchState(), SwitchState::SWITCH_STATE_RIGHT);

	flank_switches.clear();
	s5->setState(DoubleCrossSwitch::State::BC);
	QCOMPARE(s5->flank(flank_switches, true), 2u);
	QVERIFY(!contains(flank_switches, s3));
	QVERIFY( contains(flank_switches, s4));
	QVERIFY( contains(flank_switches, s6));
	QVERIFY(!contains(flank_switches, s7));
	QCOMPARE(s4->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s6->switchState(), SwitchState::SWITCH_STATE_LEFT);

	flank_switches.clear();
	s5->setState(DoubleCrossSwitch::State::BD);
	QCOMPARE(s5->flank(flank_switches, true), 2u);
	QVERIFY(!contains(flank_switches, s3));
	QVERIFY( contains(flank_switches, s4));
	QVERIFY(!contains(flank_switches, s6));
	QVERIFY( contains(flank_switches, s7));
	QCOMPARE(s4->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s7->switchState(), SwitchState::SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionRsLeft()
{
	RegularSwitch * s1 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 1, 1));
	RegularSwitch * s2 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 1, 2));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s1 != nullptr);
	QVERIFY(s2 != nullptr);

	flank_switches.clear();
	s1->setState(RegularSwitch::State::AB);
	QCOMPARE(s1->flank(flank_switches, true), 0u);
	QVERIFY(!contains(flank_switches, s2));
	QCOMPARE(s1->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s2->switchState(), SwitchState::SWITCH_STATE_LEFT);

	flank_switches.clear();
	s1->setState(RegularSwitch::State::AC);
	QCOMPARE(s1->flank(flank_switches, true), 1u);
	QVERIFY(contains(flank_switches, s2));
	QCOMPARE(s1->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s2->switchState(), SwitchState::SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionRsRight()
{
	RegularSwitch * s8 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 10, 0));
	RegularSwitch * s9 = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 11, 0));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s8 != nullptr);
	QVERIFY(s9 != nullptr);

	flank_switches.clear();
	s8->setState(RegularSwitch::State::AC);
	QCOMPARE(s8->flank(flank_switches, true), 0u);
	QVERIFY(!contains(flank_switches, s9));
	QCOMPARE(s8->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s9->switchState(), SwitchState::SWITCH_STATE_RIGHT);

	flank_switches.clear();
	s8->setState(RegularSwitch::State::AB);
	QCOMPARE(s8->flank(flank_switches, true), 1u);
	QVERIFY(contains(flank_switches, s9));
	QCOMPARE(s8->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s9->switchState(), SwitchState::SWITCH_STATE_LEFT);
}

void TestFlankSwitch::testFlankProtectionMid()
{
	AbstractSwitch * s14 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(1,  8, 0));
	AbstractSwitch * s15 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(1, 11, 0));

	QVERIFY(s14 != nullptr);
	QVERIFY(s15 != nullptr);

	QVERIFY(!s14->isFlankProtection(nullptr));
	QVERIFY(!s14->isFlankProtection(s14));
	QVERIFY(!s14->isFlankProtection(s15));

	QVERIFY(!s15->isFlankProtection(nullptr));
	QVERIFY(!s15->isFlankProtection(s14));
	QVERIFY(!s15->isFlankProtection(s15));
}

void TestFlankSwitch::testFlankProtectionEast()
{
	AbstractSwitch * s15 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(1, 11, 0));
	AbstractSwitch * s21 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(2,  1, 0));
	AbstractSwitch * s22 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(2,  1, 1));

	QVERIFY(s15 != nullptr);
	QVERIFY(s21 != nullptr);
	QVERIFY(s22 != nullptr);

	QVERIFY(!s21->isFlankProtection(nullptr));
	QVERIFY(!s21->isFlankProtection(s15));
	QVERIFY(!s21->isFlankProtection(s22));
}

void TestFlankSwitch::testFlankProtectionFactory()
{
	AbstractSwitch * s24 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(2, 11, 1));
	AbstractSwitch * s31 = dynamic_cast<AbstractSwitch *>(model->assemblyPart(3,  2, 0));

	QVERIFY(s24 != nullptr);
	QVERIFY(s31 != nullptr);

	QVERIFY(!s24->isFlankProtection(nullptr));
	QVERIFY(!s24->isFlankProtection(s31));
}

void TestFlankSwitch::testFlankProtectionParallel()
{
	RegularSwitch * s1  = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 1, 1));
	RegularSwitch * s2  = dynamic_cast<RegularSwitch *>(model->assemblyPart(0, 1, 2));
	Rail      *     r11 = dynamic_cast<Rail *>(model->assemblyPart(0,  0, 0));
	Rail      *     r16 = dynamic_cast<Rail *>(model->assemblyPart(0, 12, 0));

	QVERIFY(s1 != nullptr);
	QVERIFY(s2 != nullptr);
	QVERIFY(r11 != nullptr);
	QVERIFY(r16 != nullptr);

	s1->setState(RegularSwitch::State::AB);
	s2->setState(RegularSwitch::State::AB);

	TestRoute  route(true, SectionState::TOUR, r11);

	QVERIFY(route.append(r16));
	const std::vector<RegularSwitch *> & flank_switches = route.doFlank();

	QCOMPARE(flank_switches.size(), 2u);
	QCOMPARE(s1->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s2->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QVERIFY(!s1->reserved());
	QVERIFY(s2->reserved());
}

void TestFlankSwitch::testFlankProtectionDoubleU1()
{
	RegularSwitch * s11 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 2, 0));
	RegularSwitch * s12 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 3, 0));
	RegularSwitch * s13 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 4, 1));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s11 != nullptr);
	QVERIFY(s12 != nullptr);
	QVERIFY(s13 != nullptr);

	flank_switches.clear();
	s12->setState(RegularSwitch::State::AC);
	QCOMPARE(s12->flank(flank_switches, true), 1u);
	QVERIFY(!contains(flank_switches, s11));
	QVERIFY(contains(flank_switches, s13));
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s13->switchState(), SwitchState::SWITCH_STATE_RIGHT);

	flank_switches.clear();
	s12->setState(RegularSwitch::State::AB);
	QCOMPARE(s12->flank(flank_switches, true), 0u);
	QVERIFY(!contains(flank_switches, s11));
	QVERIFY(!contains(flank_switches, s13));
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s13->switchState(), SwitchState::SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionDoubleU2()
{
	RegularSwitch * s11 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 2, 0));
	RegularSwitch * s12 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 3, 0));
	RegularSwitch * s13 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 4, 1));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s11 != nullptr);
	QVERIFY(s12 != nullptr);
	QVERIFY(s13 != nullptr);

	flank_switches.clear();
	s11->setState(RegularSwitch::State::AB);
	QCOMPARE(s11->flank(flank_switches, true), 1u);
	QVERIFY(!contains(flank_switches, s12));
	QVERIFY(contains(flank_switches, s13));
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s13->switchState(), SwitchState::SWITCH_STATE_LEFT);

	flank_switches.clear();
	s11->setState(RegularSwitch::State::AC);
	QCOMPARE(s11->flank(flank_switches, true), 0u);
	QVERIFY(!contains(flank_switches, s11));
	QVERIFY(!contains(flank_switches, s13));
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s13->switchState(), SwitchState::SWITCH_STATE_LEFT);
}

void TestFlankSwitch::testFlankProtectionDoubleU3()
{
	RegularSwitch * s11 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 2, 0));
	RegularSwitch * s12 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 3, 0));
	RegularSwitch * s13 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 4, 1));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s11 != nullptr);
	QVERIFY(s12 != nullptr);
	QVERIFY(s13 != nullptr);

	flank_switches.clear();
	s13->setState(RegularSwitch::State::AB);
	QCOMPARE(s13->flank(flank_switches, true), 1u);
	QVERIFY(contains(flank_switches, s11));
	QVERIFY(!contains(flank_switches, s12));
	QCOMPARE(s11->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_LEFT);

	flank_switches.clear();
	s13->setState(RegularSwitch::State::AC);
	QCOMPARE(s13->flank(flank_switches, true), 1u);
	QVERIFY(!contains(flank_switches, s11));
	QVERIFY(contains(flank_switches, s12));
	QCOMPARE(s11->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionDoubleURoute()
{
	RegularSwitch * s11  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 2, 0));
	RegularSwitch * s12  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 3, 0));
	RegularSwitch * s13  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 4, 1));
	Rail      *     r111 = dynamic_cast<Rail *>(model->assemblyPart(1, 0, 0));
	Rail      *     r121 = dynamic_cast<Rail *>(model->assemblyPart(1, 1, 0));
	Rail      *     r113 = dynamic_cast<Rail *>(model->assemblyPart(1, 5, 0));
	Rail      *     r123 = dynamic_cast<Rail *>(model->assemblyPart(1, 6, 0));

	QVERIFY(s11 != nullptr);
	QVERIFY(s12 != nullptr);
	QVERIFY(s13 != nullptr);
	QVERIFY(r111 != nullptr);
	QVERIFY(r121 != nullptr);
	QVERIFY(r113 != nullptr);
	QVERIFY(r123 != nullptr);

	TestRoute  route_top(   true, SectionState::TOUR, r111);
	TestRoute  route_bottom(true, SectionState::TOUR, r121);

	// First turn bottom route and verify.
	QVERIFY(route_bottom.append(r123));
	const std::vector<RegularSwitch *> & flanks_bottom = route_bottom.doFlank();
	QCOMPARE(flanks_bottom.size(), 1u);

	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s13->switchState(), SwitchState::SWITCH_STATE_RIGHT);

	// Now turn top route and verify.
	QVERIFY(route_top.append(r113));
	const std::vector<RegularSwitch *> & flanks_top = route_top.doFlank();
	QCOMPARE(flanks_top.size(), 1u);

	QCOMPARE(s11->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s13->switchState(), SwitchState::SWITCH_STATE_LEFT);
}

void TestFlankSwitch::testFlankProtectionRhombus()
{
	RegularSwitch * s14 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  8, 0));
	RegularSwitch * s15 = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 11, 0));
	std::vector<RegularSwitch *> flank_switches;

	QVERIFY(s14 != nullptr);
	QVERIFY(s15 != nullptr);

	flank_switches.clear();
	s14->setState(RegularSwitch::State::AC);
	s15->setState(RegularSwitch::State::AB);

	QCOMPARE(s14->flank(flank_switches, true), 0u);
	QVERIFY(!contains(flank_switches, s15));
	QCOMPARE(s14->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s15->switchState(), SwitchState::SWITCH_STATE_LEFT);

	flank_switches.clear();
	s14->setState(RegularSwitch::State::AB);
	s15->setState(RegularSwitch::State::AC);
	QCOMPARE(s14->flank(flank_switches, true), 0u);
	QVERIFY(!contains(flank_switches, s15));
	QCOMPARE(s14->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s15->switchState(), SwitchState::SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionRhombusRouteLR()
{
	RegularSwitch * s11  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  2, 0));
	RegularSwitch * s12  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  3, 0));
	RegularSwitch * s13  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  4, 1));
	RegularSwitch * s14  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  8, 0));
	RegularSwitch * s15  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 11, 0));
	Rail      *     r121 = dynamic_cast<Rail *>(model->assemblyPart(1,  1, 0));
	Rail      *     r118 = dynamic_cast<Rail *>(model->assemblyPart(1, 12, 0));

	QVERIFY(s11 != nullptr);
	QVERIFY(s12 != nullptr);
	QVERIFY(s13 != nullptr);
	QVERIFY(s14 != nullptr);
	QVERIFY(s15 != nullptr);
	QVERIFY(r121 != nullptr);
	QVERIFY(r118 != nullptr);

	TestRoute  route(true, SectionState::TOUR, r121);

	QVERIFY(route.append(r118));
	const std::vector<RegularSwitch *> & flank_switches = route.doFlank();
	QCOMPARE(flank_switches.size(), 1u);
	QVERIFY(contains(flank_switches, s11));

	QCOMPARE(s11->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s13->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s14->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s15->switchState(), SwitchState::SWITCH_STATE_LEFT);
}

void TestFlankSwitch::testFlankProtectionRhombusRouteRL()
{
	RegularSwitch * s11  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  2, 0));
	RegularSwitch * s12  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  3, 0));
	RegularSwitch * s13  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  4, 1));
	RegularSwitch * s14  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1,  8, 0));
	RegularSwitch * s15  = dynamic_cast<RegularSwitch *>(model->assemblyPart(1, 11, 0));
	Rail      *     r111 = dynamic_cast<Rail *>(model->assemblyPart(1,  0, 0));
	Rail      *     r118 = dynamic_cast<Rail *>(model->assemblyPart(1, 12, 0));

	QVERIFY(s11 != nullptr);
	QVERIFY(s12 != nullptr);
	QVERIFY(s13 != nullptr);
	QVERIFY(s14 != nullptr);
	QVERIFY(s15 != nullptr);
	QVERIFY(r111 != nullptr);
	QVERIFY(r118 != nullptr);

	TestRoute  route(false, SectionState::TOUR, r118);

	QVERIFY(route.append(r111));
	const std::vector<RegularSwitch *> & flank_switches = route.doFlank();
	QCOMPARE(flank_switches.size(), 1u);
	QVERIFY(contains(flank_switches, s12));

	QCOMPARE(s11->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s12->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s13->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s14->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s15->switchState(), SwitchState::SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionFar()
{
	DoubleCrossSwitch * s21  = dynamic_cast<DoubleCrossSwitch *>(model->assemblyPart(2,  1, 0));
	RegularSwitch   *   s22  = dynamic_cast<RegularSwitch *>(    model->assemblyPart(2,  1, 1));
	RegularSwitch   *   s23  = dynamic_cast<RegularSwitch *>(    model->assemblyPart(2, 10, 0));
	Rail        *       r211 = dynamic_cast<Rail *>(model->assemblyPart(2,  0, 0));
	Rail        *       r217 = dynamic_cast<Rail *>(model->assemblyPart(2, 11, 0));

	QVERIFY(s21 != nullptr);
	QVERIFY(s22 != nullptr);
	QVERIFY(s23 != nullptr);
	QVERIFY(r211 != nullptr);
	QVERIFY(r217 != nullptr);

	s22->setState(RegularSwitch::State::AB);
	QCOMPARE(s22->switchState(), SwitchState::SWITCH_STATE_LEFT);

	TestRoute  route(true, SectionState::TOUR, r211);

	QVERIFY(route.append(r217));
	const std::vector<RegularSwitch *> & flanks = route.doFlank();
	QCOMPARE(flanks.size(), 0u);

	QCOMPARE(s21->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s22->switchState(), SwitchState::SWITCH_STATE_LEFT);
	QCOMPARE(s23->switchState(), SwitchState::SWITCH_STATE_RIGHT);
}

void TestFlankSwitch::testFlankProtectionDifferentRegion()
{
	RegularSwitch * s24  = dynamic_cast<RegularSwitch *>(model->assemblyPart(2, 11, 1));
	RegularSwitch * s31  = dynamic_cast<RegularSwitch *>(model->assemblyPart(3,  2, 0));
	Rail      *     r211 = dynamic_cast<Rail *>(model->assemblyPart(2,  0, 0));
	Rail      *     r228 = dynamic_cast<Rail *>(model->assemblyPart(2, 13, 0));

	QVERIFY(s24 != nullptr);
	QVERIFY(s31 != nullptr);
	QVERIFY(r211 != nullptr);
	QVERIFY(r228 != nullptr);

	s31->setState(RegularSwitch::State::AB);
	QCOMPARE(s31->switchState(), SwitchState::SWITCH_STATE_LEFT);
	TestRoute  route(true, SectionState::TOUR, r211);

	QVERIFY(route.append(r228));
	const std::vector<RegularSwitch *> & flanks = route.doFlank();
	QCOMPARE(flanks.size(), 0u);

	QCOMPARE(s24->switchState(), SwitchState::SWITCH_STATE_RIGHT);
	QCOMPARE(s31->switchState(), SwitchState::SWITCH_STATE_LEFT);
}
