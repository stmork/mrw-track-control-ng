//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>
#include <QDebug>

#include "testutil.h"

using namespace mrw::test;

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
	QStringList args
	{
		"MRW-Test", "-o", "qtest-model.xml", "-xml"
	};

	return EXIT_SUCCESS;
}

int main(int argc, char * argv[])
{
	Q_UNUSED(argc)
	Q_UNUSED(argv)

	int status = 0;

	status += testUtil();
	status += testModel();

	return status;
}
