//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QFile>
#include <QTest>

#include <model/switchmodulereference.h>
#include <model/lightsignal.h>

#include "testmodel.h"

using namespace mrw::test;
using namespace mrw::model;

TestModel::TestModel(const char * modelname, QObject * parent) :
	QObject(parent),
	filename(QString(modelname) + ".modelrailway")
{
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

void TestModel::testModel()
{
	QVERIFY(model != nullptr);
	QVERIFY(model->isValid());
}

void TestModel::testControllers()
{
	const size_t count = model->controllerCount();

	for (unsigned i = 0; i < count; i++)
	{
		Controller * controller = model->controller(i);

		QVERIFY(controller != nullptr);
	}

	QVERIFY_EXCEPTION_THROWN(model->controller(count), std::out_of_range);
}

void TestModel::testAreas()
{
	const size_t count = model->areaCount();

	for (unsigned i = 0; i < count; i++)
	{
		Area * area = model->area(i);

		QVERIFY(area != nullptr);
	}

	QVERIFY_EXCEPTION_THROWN(model->area(count), std::out_of_range);
}

void TestModel::testSections()
{
	const size_t area_count = model->areaCount();

	for (unsigned a = 0; a < area_count; a++)
	{
		Area * area = model->area(a);

		QVERIFY(area != nullptr);

		const size_t section_count = area->sectionCount();
		for (unsigned s = 0; s < section_count; s++)
		{
			Section * section = area->section(s);

			QVERIFY(section != nullptr);

			SectionModule * module = section->module();

			QVERIFY(module != nullptr);

			const size_t rail_count = section->railPartCount();
			for (unsigned r = 0; r < rail_count; r++)
			{
				RailPart * part = section->railPart(r);

				QVERIFY(part != nullptr);

				SwitchModuleReference * reference = dynamic_cast<SwitchModuleReference *>(part);
				LightSignal      *      signal    = dynamic_cast<LightSignal *>(part);

				if (reference != nullptr)
				{
					QVERIFY(signal == nullptr);
					QVERIFY(reference->module() != nullptr);
				}
				if (signal != nullptr)
				{
					QVERIFY(reference == nullptr);
					QVERIFY(signal->connection() != nullptr);
				}
			}
			QVERIFY_EXCEPTION_THROWN(section->railPart(rail_count), std::out_of_range);
		}

		QVERIFY_EXCEPTION_THROWN(area->section(section_count), std::out_of_range);
	}
}
