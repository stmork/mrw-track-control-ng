//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QTest>

#include <model/switchmodule.h>

#include "testswitch.h"
#include "testdef.h"

using namespace mrw::test;
using namespace mrw::model;

TestSwitch::TestSwitch() : TestModel("Test-Switch")
{
}

void TestSwitch::testSwitch()
{
	QCOMPARE(model->controllerCount(), 1u);
	QCOMPARE(model->regionCount(), 1u);

	Controller * controller = model->controller(0);

	QCOMPARE(controller->id(), TEST_SID);
}

void TestSwitch::testSwitchModule()
{
	Module * switch_module = model->module(0, 0);
	Module * rail_module   = model->module(0, 1);

	QVERIFY(switch_module != nullptr);
	QVERIFY(rail_module != nullptr);

	QCOMPARE(switch_module->id(), 1);
	QCOMPARE(rail_module->id(), 3);
}
