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

TestDoubleCrossSwitchWidget::TestDoubleCrossSwitchWidget(QObject * parent) : QObject(parent)
{
	widget.setController(&mock);
}

void TestDoubleCrossSwitchWidget::init()
{
	status = DoubleCrossSwitchWidget::Status();
}

void TestDoubleCrossSwitchWidget::testSimple()
{
	widget.prepare(status);
	QVERIFY(!status.expandable);
}

void TestDoubleCrossSwitchWidget::testPrepare()
{
	for (const LockState lock :
		{
			LockState::FAIL, LockState::UNLOCKED, LockState::PENDING, LockState::LOCKED
		})
	{
		mock.setLock(lock);
		for (const SectionState state :
			{
				FREE, SHUNTING, TOUR, OCCUPIED, PASSED
			})
		{
			mock.setSectionState(state);
			for (const bool dir :
				{
					true, false
				})
			{
				mock.setDirection(dir);
				for (const bool flank :
					{
						true, false
					})
				{
					mock.setFlankProtection(flank);

					widget.prepare(status);
					QCOMPARE(status.direction, dir);
					QCOMPARE(status.section_state, state);
					QCOMPARE(status.lock_state, lock);
					QCOMPARE(status.has_flank_protection, flank);
					QCOMPARE(status.bending, Bending::STRAIGHT);
					QCOMPARE(status.extensions, 0);
					QCOMPARE(status.lines, 0);
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
	for (const bool right_bended :
		{
			true, false
		})
	{
		mock.setRightHanded(right_bended);
		mock.setSwitchState(DoubleCrossSwitch::State::AC);
		widget.prepare(status);
		QVERIFY(status.is_a == right_bended);
		QVERIFY(status.is_b != right_bended);
		QVERIFY(status.is_c == right_bended);
		QVERIFY(status.is_d != right_bended);
	}
}

void TestDoubleCrossSwitchWidget::testSwitchStateAD()
{
	for (const bool right_bended :
		{
			true, false
		})
	{
		mock.setRightHanded(right_bended);
		mock.setSwitchState(DoubleCrossSwitch::State::AD);
		widget.prepare(status);
		QVERIFY(status.is_a == right_bended);
		QVERIFY(status.is_b != right_bended);
		QVERIFY(status.is_c != right_bended);
		QVERIFY(status.is_d == right_bended);
	}
}

void TestDoubleCrossSwitchWidget::testSwitchStateBC()
{
	for (const bool right_bended :
		{
			true, false
		})
	{
		mock.setRightHanded(right_bended);
		mock.setSwitchState(DoubleCrossSwitch::State::BC);
		widget.prepare(status);
		QVERIFY(status.is_a != right_bended);
		QVERIFY(status.is_b == right_bended);
		QVERIFY(status.is_c == right_bended);
		QVERIFY(status.is_d != right_bended);
	}
}

void TestDoubleCrossSwitchWidget::testSwitchStateBD()
{
	for (const bool right_bended :
		{
			true, false
		})
	{
		mock.setRightHanded(right_bended);
		mock.setSwitchState(DoubleCrossSwitch::State::BD);
		widget.prepare(status);
		QVERIFY(status.is_a != right_bended);
		QVERIFY(status.is_b == right_bended);
		QVERIFY(status.is_c != right_bended);
		QVERIFY(status.is_d == right_bended);
	}
}

void TestDoubleCrossSwitchWidget::testLeftHanded()
{
	mock.setLeftHanded();
	widget.prepare(status);
	QVERIFY(!status.right_bended);

	mock.setRightHanded(false);
	widget.prepare(status);
	QVERIFY(!status.right_bended);
}

void TestDoubleCrossSwitchWidget::testRightHanded()
{
	mock.setRightHanded();
	widget.prepare(status);
	QVERIFY(status.right_bended);

	mock.setLeftHanded(false);
	widget.prepare(status);
	QVERIFY(status.right_bended);
}
