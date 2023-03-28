//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTest>

#include "testrailwidget.h"

using namespace mrw::test;
using namespace mrw::model;

using LockState = Device::LockState;
using Bending   = Position::Bending;

TestRailWidget::TestRailWidget(QObject * parent) : QObject(parent)
{
	widget.setController(&mock);
}

void TestRailWidget::testPrepare()
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
			for (const Bending bending :
				{
					Bending::LEFT, Bending::STRAIGHT, Bending::RIGHT
				})
			{
				mock.setBending(bending);
				for (const bool dir :
					{
						true, false
					})
				{
					mock.setDirection(dir);
					for (int ext = 0; ext < 5; ext++)
					{
						mock.setExtension(ext);
						for (int lines = 0; lines < 5; lines++)
						{
							mock.setLines(lines);

							widget.prepare(status);
							QCOMPARE(status.direction, dir);
							QCOMPARE(status.section_state, state);
							QCOMPARE(status.lock_state, lock);
							QCOMPARE(status.bending, bending);
							QCOMPARE(status.extensions, ext);
							QCOMPARE(status.lines, lines);
						}
					}
				}
			}
		}
	}
}

void TestRailWidget::testHavingLock()
{
	QVERIFY(!widget.hasLock());
}
