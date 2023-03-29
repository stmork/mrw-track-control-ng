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

TestRegularSwitchWidget::TestRegularSwitchWidget(QObject * parent) :
	QObject(parent), widget(mock)
{
}

void TestRegularSwitchWidget::init()
{
	status = RegularSwitchWidget::Status();
}

void TestRegularSwitchWidget::testSimple()
{
	widget.test(status);
	QVERIFY(status.expandable);
}

void TestRegularSwitchWidget::testPrepare()
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
				for (const bool inclination : booleans)
				{
					mock.setInclined(inclination);
					for (const bool flank : booleans)
					{
						mock.setFlankProtection(flank);
						for (int ext = 0; ext < 5; ext++)
						{
							mock.setExtension(ext);

							widget.test(status);
							QCOMPARE(status.direction, dir);
							QCOMPARE(status.section_state, state);
							QCOMPARE(status.lock_state, lock);
							QCOMPARE(status.has_flank_protection, flank);
							QCOMPARE(status.extensions, ext);
							QCOMPARE(status.lines, 0.0f);
							QCOMPARE(status.inclined, inclination);
							QCOMPARE(widget.connectors().size(), inclination ? 2 : 1);
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

void TestRegularSwitchWidget::testLeft()
{
	for (const bool inclination : booleans)
	{
		const int connections = inclination ? 2 : 1;

		mock.setInclined(inclination);
		mock.setLeft();
		widget.test(status);
		QVERIFY(status.left);
		QVERIFY(!status.right);
		QCOMPARE(widget.connectors().size(), connections);

		mock.setRight(false);
		widget.test(status);
		QVERIFY(status.left);
		QVERIFY(!status.right);
		QCOMPARE(widget.connectors().size(), connections);
	}
}

void TestRegularSwitchWidget::testRight()
{
	for (const bool inclination : booleans)
	{
		const int connections = inclination ? 2 : 1;

		mock.setInclined(inclination);
		mock.setRight();
		widget.test(status);
		QVERIFY(!status.left);
		QVERIFY(status.right);
		QCOMPARE(widget.connectors().size(), connections);

		mock.setLeft(false);
		widget.test(status);
		QVERIFY(!status.left);
		QVERIFY(status.right);
		QCOMPARE(widget.connectors().size(), connections);
	}
}

void TestRegularSwitchWidget::testBending()
{
	for (const bool inclination : booleans)
	{
		const int connections = inclination ? 2 : 1;

		mock.setInclined(inclination);
		mock.setLeftHanded();
		widget.test(status);
		QCOMPARE(status.bending, Bending::LEFT);
		QCOMPARE(widget.connectors().size(), connections);

		mock.setRightHanded();
		widget.test(status);
		QCOMPARE(status.bending, Bending::RIGHT);
		QCOMPARE(widget.connectors().size(), connections);

		mock.setLeftHanded(false);
		widget.test(status);
		QCOMPARE(status.bending, Bending::RIGHT);
		QCOMPARE(widget.connectors().size(), connections);

		mock.setRightHanded(false);
		widget.test(status);
		QCOMPARE(status.bending, Bending::LEFT);
		QCOMPARE(widget.connectors().size(), connections);
	}
}
