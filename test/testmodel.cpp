//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>

#include <can/commands.h>
#include <model/switchmodulereference.h>
#include <model/controller.h>
#include <model/lightsignal.h>
#include <model/railpart.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>
#include <model/switchmodule.h>

#include "testmodel.h"

using namespace mrw::can;
using namespace mrw::test;
using namespace mrw::model;

TestModel::TestModel(const char * modelname, QObject * parent) :
	TestModelBase(modelname, parent)
{
}

void TestModel::testModel()
{
	QVERIFY(model != nullptr);
	QVERIFY(model->valid());
	model->info();
	model->dump();
}

void TestModel::testControllers()
{
	const size_t count = model->controllerCount();

	for (unsigned c = 0; c < count; c++)
	{
		Controller * controller = model->controller(c);

		QVERIFY(controller != nullptr);
		QVERIFY(controller->valid());
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
		QVERIFY_EXCEPTION_THROWN(controller->module(module_count), std::out_of_range);

		const size_t mux_count = controller->connectionCount();
		for (unsigned m = 0; m < mux_count; m++)
		{
			MultiplexConnection * module = controller->connection(m);

			testMuxConnection(module);
		}
		QVERIFY_EXCEPTION_THROWN(controller->connection(mux_count), std::out_of_range);
	}
}

void TestModel::testModule(Module * module)
{
	QVERIFY(module != nullptr);
	QVERIFY(module->valid());
	QVERIFY(module->id() != 0);
}

void TestModel::testMuxConnection(MultiplexConnection * connection)
{
	QVERIFY(connection != nullptr);
	QVERIFY(connection->valid());
	QVERIFY(connection->id() != 0);
}

void TestModel::testRegions()
{
	const size_t count = model->regionCount();

	for (unsigned i = 0; i < count; i++)
	{
		Region * region = model->region(i);

		QVERIFY(region != nullptr);
		QVERIFY(region->key().contains(region->name()));
	}

	QVERIFY_EXCEPTION_THROWN(model->region(count), std::out_of_range);
}

void TestModel::testSections()
{
	const size_t region_count = model->regionCount();

	for (unsigned a = 0; a < region_count; a++)
	{
		Region * region = model->region(a);

		QVERIFY(region != nullptr);

		const size_t section_count = region->sectionCount();
		for (unsigned s = 0; s < section_count; s++)
		{
			Section * section = region->section(s);

			testSection(section);
		}

		QVERIFY_EXCEPTION_THROWN(region->section(section_count), std::out_of_range);
	}
}

void TestModel::testRegularSwitchStates()
{
	std::vector<RegularSwitch *> switches;

	model->parts<RegularSwitch>(switches);
	for (RegularSwitch * part : switches)
	{
		part->setState(RegularSwitch::State::AB);
		QCOMPARE(part->state(), RegularSwitch::State::AB);
		QCOMPARE(part->commandState(), SwitchState::SWITCH_STATE_LEFT);

		part->setState(RegularSwitch::State::AC);
		QCOMPARE(part->state(), RegularSwitch::State::AC);
		QCOMPARE(part->commandState(), SwitchState::SWITCH_STATE_RIGHT);
	}
}

void TestModel::tesDoubleCrossSwitchStates()
{
	std::vector<DoubleCrossSwitch *> switches;

	model->parts<DoubleCrossSwitch>(switches);
	for (DoubleCrossSwitch * part : switches)
	{
		part->setState(DoubleCrossSwitch::State::AC);
		QCOMPARE(part->state(), DoubleCrossSwitch::State::AC);
		QCOMPARE(part->commandState(), SwitchState::SWITCH_STATE_LEFT);

		part->setState(DoubleCrossSwitch::State::AD);
		QCOMPARE(part->state(), DoubleCrossSwitch::State::AD);
		QCOMPARE(part->commandState(), SwitchState::SWITCH_STATE_RIGHT);

		part->setState(DoubleCrossSwitch::State::BC);
		QCOMPARE(part->state(), DoubleCrossSwitch::State::BC);
		QCOMPARE(part->commandState(), SwitchState::SWITCH_STATE_RIGHT);

		part->setState(DoubleCrossSwitch::State::BD);
		QCOMPARE(part->state(), DoubleCrossSwitch::State::BD);
		QCOMPARE(part->commandState(), SwitchState::SWITCH_STATE_LEFT);
	}
}

void TestModel::testSection(Section * section)
{
	QVERIFY(section != nullptr);
	QVERIFY(section->valid());
	QVERIFY(section->unitNo() != 0);
	QVERIFY(section->controller() != nullptr);

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
	const std::string   &   name      = part->toString().toStdString();

	QVERIFY2(part->valid(), name.c_str());

	if (reference != nullptr)
	{
		QVERIFY2(signal == nullptr, name.c_str());
		QVERIFY2(device != nullptr, name.c_str());
		QVERIFY2(reference->module() != nullptr, name.c_str());
		QVERIFY2(reference->controller() != nullptr, name.c_str());
	}

	if (signal != nullptr)
	{
		QVERIFY2(reference == nullptr, name.c_str());
		QVERIFY2(device != nullptr, name.c_str());
		QVERIFY2(rail == nullptr, name.c_str());
		QVERIFY2(signal->connection() != nullptr, name.c_str());
		QVERIFY2(signal->controller() != nullptr, name.c_str());
	}

	if (rail != nullptr)
	{
		QVERIFY2(signal == nullptr, name.c_str());
		QVERIFY2(rail->valid(), name.c_str());
		QVERIFY2(rail->key().contains(rail->partName()), name.c_str());
	}

	if (device != nullptr)
	{
		QVERIFY2(device->unitNo() != 0, name.c_str());
		QVERIFY2(device->controller() != nullptr, name.c_str());
	}
}
