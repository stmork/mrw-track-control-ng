//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QTest>

#include <can/mrwmessage.h>
#include <model/regularswitch.h>

#include "testunknown.h"
#include "testdef.h"

using namespace mrw::test;
using namespace mrw::can;
using namespace mrw::model;

#if (QT_VERSION < QT_VERSION_CHECK(6, 3, 0))
#	define MRW_THROWS_EXCEPTION(condition, exception) QVERIFY_EXCEPTION_THROWN(condition, exception);
#else
#	define MRW_THROWS_EXCEPTION(condition, exception) QVERIFY_THROWS_EXCEPTION(exception, condition);
#endif

TestUnknown::TestUnknown() : TestModelBase("Test-Unknown")
{
}

void TestUnknown::testInvalid()
{
	QVERIFY(!model->valid());
}

void TestUnknown::testController()
{
	QVERIFY(!model->controller(0)->valid());
	QVERIFY( model->controller(1)->valid());
	QVERIFY(!model->controller(2)->valid());
	QVERIFY(!model->controller(3)->valid());

	std::vector<MrwMessage> messages;
	MRW_THROWS_EXCEPTION(model->controller(3)->configure(messages), std::logic_error);
}

void TestUnknown::testModule()
{
	QVERIFY( model->module(0, 0)->valid());
	QVERIFY(!model->module(0, 1)->valid());
}

void TestUnknown::testMuxConnection()
{
	QVERIFY(!model->connection(0, 0)->valid());
	QVERIFY( model->connection(1, 0)->valid());
	QVERIFY(!model->connection(2, 0)->valid());
	QVERIFY(!model->connection(3, 0)->valid());
}
