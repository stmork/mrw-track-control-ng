//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <QTest>

#include <model/rail.h>
#include <model/route.h>

#include "testcrossing.h"

using namespace mrw::test;
using namespace mrw::model;

TestCrossing::TestCrossing() : TestModelBase("Test-Crossing")
{
}

void TestCrossing::testSimple()
{
	QVERIFY(model->valid());
	QVERIFY(model->controller(0)->valid());
	QVERIFY(model->controller(1)->valid());
	QVERIFY(model->connection(0, 0)->valid());

	QVERIFY(parts[0]->section()->crossing() == nullptr);
	QVERIFY(parts[1]->section()->crossing() == nullptr);
	QVERIFY(parts[2]->section()->crossing() == nullptr);
	QVERIFY(parts[3]->section()->crossing() == nullptr);
	QVERIFY(parts[4]->section()->crossing() != nullptr);
	QVERIFY(parts[5]->section()->crossing() != nullptr);
	QVERIFY(parts[6]->section()->crossing() == nullptr);
	QVERIFY(parts[7]->section()->crossing() == nullptr);
	QVERIFY(parts[8]->section()->crossing() == nullptr);
	QVERIFY(parts[9]->section()->crossing() == nullptr);
}

void TestCrossing::testDown()
{
	Crossing * crossing = model->connection(0, 0)->crossings().at(0);

	QVERIFY(crossing != nullptr);
	QVERIFY(crossing->name().size() > 0);
	QVERIFY(crossing->isUnlockable());
	QVERIFY(!crossing->isUsed());
}

void TestCrossing::testUpper()
{
	Crossing * crossing = model->connection(0, 0)->crossings().at(0);
	Rail   *   r11      = dynamic_cast<Rail *>(parts[0]);
	Rail   *   r15      = dynamic_cast<Rail *>(parts[8]);

	QVERIFY(crossing != nullptr);
	QVERIFY(!crossing->isUsed());
	QVERIFY(r11 != nullptr);
	QVERIFY(r15 != nullptr);

	Route   route1(false, SectionState::TOUR, r15);
	QVERIFY(route1.append(r11));
	QVERIFY(crossing->isUsed());
}

void TestCrossing::testLower()
{
	Crossing * crossing = model->connection(0, 0)->crossings().at(0);
	Rail   *   r21      = dynamic_cast<Rail *>(parts[1]);
	Rail   *   r25      = dynamic_cast<Rail *>(parts[9]);

	QVERIFY(crossing != nullptr);
	QVERIFY(!crossing->isUsed());
	QVERIFY(r21 != nullptr);
	QVERIFY(r25 != nullptr);

	Route   route2(true, SectionState::TOUR, r21);
	QVERIFY(route2.append(r25));
	QVERIFY(crossing->isUsed());
}

void TestCrossing::testBoth()
{
	Crossing * crossing = model->connection(0, 0)->crossings().at(0);
	Rail   *   r11      = dynamic_cast<Rail *>(parts[0]);
	Rail   *   r15      = dynamic_cast<Rail *>(parts[8]);
	Rail   *   r21      = dynamic_cast<Rail *>(parts[1]);
	Rail   *   r25      = dynamic_cast<Rail *>(parts[9]);

	QVERIFY(crossing != nullptr);
	QVERIFY(!crossing->isUsed());
	QVERIFY(r11 != nullptr);
	QVERIFY(r15 != nullptr);
	QVERIFY(r21 != nullptr);
	QVERIFY(r25 != nullptr);

	Route   route1(false, SectionState::TOUR, r15);
	QVERIFY(route1.append(r11));
	QVERIFY(crossing->isUsed());

	Route   route2(true,  SectionState::TOUR, r21);
	QVERIFY(route2.append(r25));
	QVERIFY(crossing->isUsed());
}
