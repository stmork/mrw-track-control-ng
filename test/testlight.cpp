//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>

#include "testlight.h"

using namespace mrw::test;
using namespace mrw::model;

TestLight::TestLight() : TestModel("Test-Light")
{
}

void TestLight::testLight()
{
	QCOMPARE(model->controllerCount(), 1);
	QCOMPARE(model->areaCount(), 2);

	Controller * controller = model->controller(0);

	QCOMPARE(controller->id(), 2040);
}

void TestLight::testLightModule()
{
}
