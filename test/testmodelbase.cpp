//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QFile>
#include <QTest>

#include "testmodelbase.h"

using namespace mrw::test;
using namespace mrw::model;

TestModelBase::TestModelBase(const char * modelname, QObject * parent) :
	QObject(parent),
	filename(QString(modelname) + ".modelrailway")
{
	if (!QFile::exists(filename))
	{
		filename = "test/" + filename;
	}
}

void TestModelBase::initTestCase()
{
	model = new ModelRailway(filename);
	QVERIFY(model != nullptr);

	model->parts<RailPart>(parts);
}

void TestModelBase::cleanupTestCase()
{
	delete model;
	model = nullptr;
}

void TestModelBase::testInfo()
{
	model->info();
	model->xml();
}
