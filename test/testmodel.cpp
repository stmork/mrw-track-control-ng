//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QFile>
#include <QTest>

#include "testmodel.h"

using namespace mrw::test;
using namespace mrw::model;

TestModel::TestModel(const char * modelname, QObject * parent) :
	QObject(parent)
{
	filename = QString(modelname) + ".modelrailway";
	if (!QFile::exists(filename))
	{
		filename = "test/" + filename;
	}
}

void TestModel::initTestCase()
{
	model = new ModelRailway(filename);
	QVERIFY(model != nullptr);
}

void TestModel::cleanupTestCase()
{
	delete model;
	model = nullptr;
}

void TestModel::testController()
{
	const size_t count = model->controllerCount();

	for(unsigned i = 0; i < count; i++)
	{
		Controller * controller = model->controller(i);

		QVERIFY(controller != nullptr);
		QCOMPARE(controller->id(), 101 + i);
	}

	QVERIFY_EXCEPTION_THROWN(model->controller(count), std::out_of_range);
}

void TestModel::testArea()
{
	const size_t count = model->areaCount();

	for(unsigned i = 0; i < count; i++)
	{
		Area * area = model->area(i);

		QVERIFY(area != nullptr);
	}

	QVERIFY_EXCEPTION_THROWN(model->area(count), std::out_of_range);
}
