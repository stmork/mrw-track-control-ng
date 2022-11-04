//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QFile>
#include <QTest>

#include <model/switchmodulereference.h>
#include <model/lightsignal.h>
#include <model/railpart.h>
#include <model/controller.h>

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

	for (unsigned c = 0; c < count; c++)
	{
		Controller * controller = model->controller(c);

		QVERIFY(controller != nullptr);
		QVERIFY(controller->id() != 0);
	}

	QVERIFY_EXCEPTION_THROWN(model->controller(count), std::out_of_range);
}

void TestModel::testModules()
{
	const size_t count = model->controllerCount();

	for (unsigned c = 0; c < count; c++)
	{
		Controller * controller = model->controller(c);

		QVERIFY(controller != nullptr);

		const size_t module_count = controller->moduleCount();
		for (unsigned m = 0; m < module_count; m++)
		{
			Module * module = controller->module(m);

			testModule(module);
		}
	}
}

void TestModel::testModule(Module * module)
{
	QVERIFY(module != nullptr);
	QVERIFY(module->id() != 0);
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

	QVERIFY2(module != nullptr, section->name().toStdString().c_str());

	const size_t rail_count = section->railPartCount();
	for (unsigned r = 0; r < rail_count; r++)
	{
		AssemblyPart * part = section->assemblyPart(r);

		testAssemblyPart(part);
	}
	QVERIFY_EXCEPTION_THROWN(section->assemblyPart(rail_count), std::out_of_range);
}

void TestModel::testAssemblyPart(AssemblyPart * part)
{
	QVERIFY(part != nullptr);

	SwitchModuleReference * reference = dynamic_cast<SwitchModuleReference *>(part);
	LightSignal      *      signal    = dynamic_cast<LightSignal *>(part);
	RailPart        *       rail      = dynamic_cast<RailPart *>(part);
	Device         *        device    = dynamic_cast<Device *>(part);
	const std::string   &   name      = part->name().toStdString();

	if (reference != nullptr)
	{
		QVERIFY2(signal == nullptr, name.c_str());
		QVERIFY2(device != nullptr, name.c_str());
		QVERIFY2(reference->module() != nullptr, name.c_str());
		QVERIFY2(device->id() != 0, name.c_str());
	}

	if (signal != nullptr)
	{
		QVERIFY2(reference == nullptr, name.c_str());
		QVERIFY2(device != nullptr, name.c_str());
		QVERIFY2(rail == nullptr, name.c_str());
		QVERIFY2(signal->connection() != nullptr, name.c_str());
		QVERIFY2(device->id() != 0, name.c_str());
	}

	if (rail != nullptr)
	{
		QVERIFY2(signal == nullptr, name.c_str());
		QVERIFY2(rail->valid(), name.c_str());
	}
}
