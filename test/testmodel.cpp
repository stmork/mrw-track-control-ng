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

			testSection(section);
		}

		QVERIFY_EXCEPTION_THROWN(area->section(section_count), std::out_of_range);
	}
}

void TestModel::testSection(Section * section)
{
	QVERIFY(section != nullptr);
	QVERIFY(section->id() != 0);

	SectionModule * module = section->module();

	QVERIFY(module != nullptr);

	const size_t rail_count = section->railPartCount();
	for (unsigned r = 0; r < rail_count; r++)
	{
		AssemblyPart * part = section->railPart(r);

		testAssemblyPart(part);
	}
	QVERIFY_EXCEPTION_THROWN(section->railPart(rail_count), std::out_of_range);
}

void TestModel::testAssemblyPart(AssemblyPart * part)
{
	QVERIFY(part != nullptr);

	SwitchModuleReference * reference = dynamic_cast<SwitchModuleReference *>(part);
	LightSignal      *      signal    = dynamic_cast<LightSignal *>(part);
	Device         *        device    = dynamic_cast<Device *>(part);
	const char       *      name      = part->name().toStdString().c_str();

	if (reference != nullptr)
	{
		QVERIFY2(signal == nullptr, name);
		QVERIFY2(device != nullptr, name);
		QVERIFY2(reference->module() != nullptr, name);
		QVERIFY2(device->id() != 0, name);
	}

	if (signal != nullptr)
	{
		QVERIFY2(reference == nullptr, name);
		QVERIFY2(device != nullptr, name);
		QVERIFY2(signal->connection() != nullptr, name);
		QVERIFY2(device->id() != 0, name);
	}
}
