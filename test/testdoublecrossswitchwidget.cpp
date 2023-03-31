//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTest>

#include "testdoublecrossswitchwidget.h"

using namespace mrw::test;
using namespace mrw::model;
using namespace mrw::ui;

using LockState = Device::LockState;
using Bending   = Position::Bending;

TestDoubleCrossSwitchWidget::TestDoubleCrossSwitchWidget(QObject * parent) :
	QObject(parent), widget(mock)
{
}

void TestDoubleCrossSwitchWidget::init()
{
	status = DoubleCrossSwitchWidget::Status();
	mock.setDirection();
	mock.setSectionState(FREE);
	mock.setLock(LockState::UNLOCKED);
	mock.setSwitchState(DoubleCrossSwitch::State::BC);
	mock.setRightHanded();
	mock.setFlankProtection(false);
}

void TestDoubleCrossSwitchWidget::testSimple()
{
	widget.test(status);
	QVERIFY(!status.expandable);
}

void TestDoubleCrossSwitchWidget::testPrepare()
{
	for (const LockState lock : lock_states)
	{
		mock.setLock(lock);
		for (const SectionState state : section_states)
		{
			mock.setSectionState(state);
			for (const bool dir : booleans)
			{
				mock.setDirection(dir);
				for (const bool flank : booleans)
				{
					mock.setFlankProtection(flank);

					widget.test(status);
					QCOMPARE(status.direction, dir);
					QCOMPARE(status.section_state, state);
					QCOMPARE(status.lock_state, lock);
					QCOMPARE(status.has_flank_protection, flank);
					QCOMPARE(status.bending, Bending::STRAIGHT);
					QCOMPARE(status.extensions, 0.0f);
					QCOMPARE(status.lines, 0.0f);
					QCOMPARE(widget.connectors().size(), 2);
				}
			}
		}
	}
}

void TestDoubleCrossSwitchWidget::testHavingLock()
{
	QVERIFY(widget.hasLock());
}

void TestDoubleCrossSwitchWidget::testSwitchStateAC()
{
	for (const bool right_bended : booleans)
	{
		mock.setRightHanded(right_bended);
		mock.setSwitchState(DoubleCrossSwitch::State::AC);
		widget.test(status);
		QVERIFY(status.is_a == right_bended);
		QVERIFY(status.is_b != right_bended);
		QVERIFY(status.is_c == right_bended);
		QVERIFY(status.is_d != right_bended);
		QCOMPARE(status.right_bended, right_bended);
		QCOMPARE(widget.connectors().size(), 2);
	}
}

void TestDoubleCrossSwitchWidget::testSwitchStateAD()
{
	for (const bool right_bended : booleans)
	{
		mock.setRightHanded(right_bended);
		mock.setSwitchState(DoubleCrossSwitch::State::AD);
		widget.test(status);
		QVERIFY(status.is_a == right_bended);
		QVERIFY(status.is_b != right_bended);
		QVERIFY(status.is_c != right_bended);
		QVERIFY(status.is_d == right_bended);
		QCOMPARE(status.right_bended, right_bended);
		QCOMPARE(widget.connectors().size(), 2);
	}
}

void TestDoubleCrossSwitchWidget::testSwitchStateBC()
{
	for (const bool right_bended : booleans)
	{
		mock.setRightHanded(right_bended);
		mock.setSwitchState(DoubleCrossSwitch::State::BC);
		widget.test(status);
		QVERIFY(status.is_a != right_bended);
		QVERIFY(status.is_b == right_bended);
		QVERIFY(status.is_c == right_bended);
		QVERIFY(status.is_d != right_bended);
		QCOMPARE(status.right_bended, right_bended);
		QCOMPARE(widget.connectors().size(), 2);
	}
}

void TestDoubleCrossSwitchWidget::testSwitchStateBD()
{
	for (const bool right_bended : booleans)
	{
		mock.setRightHanded(right_bended);
		mock.setSwitchState(DoubleCrossSwitch::State::BD);
		widget.test(status);
		QVERIFY(status.is_a != right_bended);
		QVERIFY(status.is_b == right_bended);
		QVERIFY(status.is_c != right_bended);
		QVERIFY(status.is_d == right_bended);
		QCOMPARE(status.right_bended, right_bended);
		QCOMPARE(widget.connectors().size(), 2);
	}
}

void TestDoubleCrossSwitchWidget::testLeftHanded()
{
	mock.setLeftHanded();
	widget.test(status);
	QVERIFY(!status.right_bended);
	QCOMPARE(widget.connectors().size(), 2);

	mock.setRightHanded(false);
	widget.test(status);
	QVERIFY(!status.right_bended);
	QCOMPARE(widget.connectors().size(), 2);
}

void TestDoubleCrossSwitchWidget::testRightHanded()
{
	mock.setRightHanded();
	widget.test(status);
	QVERIFY(status.right_bended);
	QCOMPARE(widget.connectors().size(), 2);

	mock.setLeftHanded(false);
	widget.test(status);
	QVERIFY(status.right_bended);
	QCOMPARE(widget.connectors().size(), 2);
}
