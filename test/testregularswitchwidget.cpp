//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTest>

#include "testregularswitchwidget.h"

using namespace mrw::test;
using namespace mrw::model;
using namespace mrw::ui;

using LockState = Device::LockState;
using Bending   = Position::Bending;

TestRegularSwitchWidget::TestRegularSwitchWidget(QObject * parent) : QObject(parent)
{
	widget.setController(&mock);
}

void TestRegularSwitchWidget::init()
{
	status = RegularSwitchWidget::Status();
}

void TestRegularSwitchWidget::testSimple()
{
	widget.prepare(status);
	QVERIFY(status.expandable);
}

void TestRegularSwitchWidget::testPrepare()
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
				for (const bool inclination :
					{
						true, false
					})
				{
					mock.setInclined(inclination);
					for (const bool flank :
						{
							true, false
						})
					{
						mock.setFlankProtection(flank);
						for (int ext = 0; ext < 5; ext++)
						{
							mock.setExtension(ext);

							widget.prepare(status);
							QCOMPARE(status.direction, dir);
							QCOMPARE(status.section_state, state);
							QCOMPARE(status.lock_state, lock);
							QCOMPARE(status.has_flank_protection, flank);
							QCOMPARE(status.extensions, ext);
							QCOMPARE(status.lines, 0);
							QCOMPARE(status.inclined, inclination);
						}
					}
				}
			}
		}
	}
}

void TestRegularSwitchWidget::testHavingLock()
{
	QVERIFY(widget.hasLock());
}

void TestRegularSwitchWidget::testBending()
{
	mock.setLeftHanded();
	widget.prepare(status);
	QCOMPARE(status.bending, Bending::LEFT);

	mock.setRightHanded();
	widget.prepare(status);
	QCOMPARE(status.bending, Bending::RIGHT);

	mock.setLeftHanded(false);
	widget.prepare(status);
	QCOMPARE(status.bending, Bending::RIGHT);

	mock.setRightHanded(false);
	widget.prepare(status);
	QCOMPARE(status.bending, Bending::LEFT);
}
