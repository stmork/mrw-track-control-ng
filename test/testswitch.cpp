//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>

#include "testswitch.h"

using namespace mrw::test;
using namespace mrw::model;

TestSwitch::TestSwitch() : TestModel("Test-Switch")
{

}

void TestSwitch::testSwitch()
{
	QCOMPARE(model->controllerCount(), 1);
	QCOMPARE(model->areaCount(), 2);

	Controller * controller = model->controller(0);

	QCOMPARE(controller->id(), 2040);
}

void TestSwitch::testSwitchModule()
{
}
