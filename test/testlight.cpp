//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>

#include "testlight.h"
#include "testdef.h"

using namespace mrw::test;
using namespace mrw::model;

TestLight::TestLight() : TestModel("Test-Light")
{
}

void TestLight::testLight()
{
	QCOMPARE(model->controllerCount(), 1u);
	QCOMPARE(model->areaCount(), 2u);

	Controller * controller = model->controller(0);

	QCOMPARE(controller->id(), TEST_SID);
}

void TestLight::testLightModule()
{
	Module * light_module = model->module(0, 0);
	Module * rail_module  = model->module(0, 1);

	QVERIFY(light_module != nullptr);
	QVERIFY(rail_module != nullptr);

	QCOMPARE(light_module->id(), 1);
	QCOMPARE(rail_module->id(), 3);
}
