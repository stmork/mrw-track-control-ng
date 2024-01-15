//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QTest>

#include "testrailwidget.h"

using namespace mrw::test;
using namespace mrw::model;
using namespace mrw::ui;

using Bending   = Position::Bending;
using LockState = Device::LockState;

TestRailWidget::TestRailWidget(QObject * parent) :
	QObject(parent), widget(mock)
{
}

void TestRailWidget::init()
{
	status = RailWidget::Status();
	mock.reset();
}

void TestRailWidget::testSimple()
{
	widget.test(status);
	QVERIFY(status.expandable);
}

void TestRailWidget::testPrepare()
{
	for (const LockState lock : lock_states)
	{
		mock.setLock(lock);
		for (const SectionState state : section_states)
		{
			mock.setSectionState(state);
			for (const Bending bending : bendings)
			{
				mock.setBending(bending);
				for (const bool dir : booleans)
				{
					mock.setDirection(dir);
					for (int ext = 0; ext < 5; ext++)
					{
						mock.setExtension(ext);
						for (int lines = 0; lines < 5; lines++)
						{
							mock.setLines(lines);

							widget.test(status);
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

void TestRailWidget::testEnds()
{
	mock.setLines(0);
	mock.setExtension(Position::FRACTION);
	for (const bool dir : booleans)
	{
		mock.setDirection(dir);
		for (const Bending bending : bendings)
		{
			const int connections = bending != Bending::STRAIGHT ? 1 : 0;
			mock.setBending(bending);

			mock.setEnds(false, false);
			widget.test(status);
			QVERIFY(!status.a_ends);
			QVERIFY(!status.b_ends);
			QVERIFY(!status.any_end);
			QCOMPARE(widget.connectors().size(), connections);

			mock.setEnds(true, false);
			widget.test(status);
			QVERIFY( status.a_ends);
			QVERIFY(!status.b_ends);
			QVERIFY( status.any_end);
			QCOMPARE(widget.connectors().size(), 0);

			mock.setEnds(false, true);
			widget.test(status);
			QVERIFY(!status.a_ends);
			QVERIFY( status.b_ends);
			QVERIFY( status.any_end);
			QCOMPARE(widget.connectors().size(), connections);

			mock.setEnds(true, true);
			widget.test(status);
			QVERIFY(status.a_ends);
			QVERIFY(status.b_ends);
			QVERIFY(status.any_end);
			QCOMPARE(widget.connectors().size(), 0);
		}
	}
}
