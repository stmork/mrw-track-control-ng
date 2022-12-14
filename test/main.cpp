//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>
#include <QTest>
#include <QDebug>

#include <util/stringutil.h>
#include <util/clockservice.h>

#include "testutil.h"
#include "testcan.h"
#include "testmodel.h"
#include "testswitch.h"
#include "testlight.h"
#include "testflankswitch.h"
#include "testnumbering.h"
#include "testunknown.h"
#include "testrouting.h"

using namespace mrw::test;
using namespace mrw::util;

static int testUtil()
{
	TestUtil    test;
	QStringList args
	{
		"MRW-Test", "-o", "qtest-util.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testCan()
{
	TestCan        test;
	QStringList    args
	{
		"MRW-Test", "-o", "qtest-can.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testModel()
{
	TestModel   test("Test-Railway");
	QStringList args
	{
		"MRW-Test", "-o", "qtest-model.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testSimpleSwitch()
{
	TestSwitch  test;
	QStringList args
	{
		"MRW-Test", "-o", "qtest-switch.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testSimpleLight()
{
	TestLight   test;
	QStringList args
	{
		"MRW-Test", "-o", "qtest-light.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testUnknown()
{
	TestUnknown    test;
	QStringList    args
	{
		"MRW-Test", "-o", "qtest-unknown.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testNumbering()
{
	TestNumbering  test;
	QStringList    args
	{
		"MRW-Test", "-o", "qtest-numbering.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testFlankSwitch()
{
	TestFlankSwitch test;
	QStringList     args
	{
		"MRW-Test", "-o", "qtest-flank.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testRouting()
{
	TestRouting     test;
	QStringList     args
	{
		"MRW-Test", "-o", "qtest-route.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

int main(int argc, char * argv[])
{
	QCoreApplication  app(argc, argv);
	Q_UNUSED(argc)
	Q_UNUSED(argv)

	int status = 0;

	status += testUtil();
	status += testCan();
	status += testModel();
	status += testSimpleSwitch();
	status += testSimpleLight();
	status += testNumbering();
	status += testUnknown();
	status += testFlankSwitch();
	status += testRouting();

	if (status == 0)
	{
		qDebug().noquote() << "All tests" << String::format(
				String::GREEN_ON + String::BOLD_ON, "OK!");
	}
	else
	{
		qDebug().noquote() << status << "tests" << String::format(
				String::RED_ON + String::BOLD_ON, "failed!");
	}

	return status;
}
