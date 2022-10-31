//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>
#include <QDebug>

#include "testmodel.h"
#include "testswitch.h"
#include "testutil.h"

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
	TestSwitch test;
	QStringList args
	{
		"MRW-Test", "-o", "qtest-switch.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

static int testSimpleLight()
{
	TestSwitch test;
	QStringList args
	{
		"MRW-Test", "-o", "qtest-light.xml", "-xml"
	};

	return QTest::qExec(&test, args);
}

int main(int argc, char * argv[])
{
	Q_UNUSED(argc)
	Q_UNUSED(argv)

	int status = 0;

	status += testUtil();
	status += testModel();
	status += testSimpleSwitch();
	status += testSimpleLight();

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
