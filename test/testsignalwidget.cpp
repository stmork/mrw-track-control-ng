//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTest>

#include "testsignalwidget.h"

using namespace mrw::test;
using namespace mrw::model;

using LockState = Device::LockState;
using Bending   = Position::Bending;

TestSignalWidget::TestSignalWidget(QObject * parent) : QObject(parent)
{
	widget.setController(&mock);
}

void TestSignalWidget::testPrepare()
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

						widget.prepare(status);
						QCOMPARE(status.direction, dir);
						QCOMPARE(status.section_state, state);
						QCOMPARE(status.lock_state, lock);
						QCOMPARE(status.bending, bending);
						QCOMPARE(status.extensions, ext);
						QCOMPARE(status.lines, 0);
					}
				}
			}
		}
	}
}
