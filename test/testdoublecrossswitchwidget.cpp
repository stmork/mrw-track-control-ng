//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTest>

#include "testdoublecrossswitchwidget.h"

using namespace mrw::test;
using namespace mrw::model;

using LockState = Device::LockState;
using Bending   = Position::Bending;

TestDoubleCrossSwitchWidget::TestDoubleCrossSwitchWidget(QObject * parent) : QObject(parent)
{
	widget.setController(&mock);
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
