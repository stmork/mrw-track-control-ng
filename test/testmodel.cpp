//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QTest>
#include <QStringLiteral>

#include <can/mrwmessage.h>
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

using Bending = Position::Bending;

TestModel::TestModel(const char * modelname, QObject * parent) :
	TestModelBase(modelname, parent)
{
}

void TestModel::testModel()
{
	QVERIFY(model != nullptr);
	QVERIFY(model->valid());
	model->info();
	model->info();
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

			testSection(region, section);
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
		QCOMPARE(part->switchState(), SwitchState::SWITCH_STATE_LEFT);
		QCOMPARE(part->commandState(), Command::SETLFT);

		part->setState(RegularSwitch::State::AC);
		QCOMPARE(part->state(), RegularSwitch::State::AC);
		QCOMPARE(part->switchState(), SwitchState::SWITCH_STATE_RIGHT);
		QCOMPARE(part->commandState(), Command::SETRGT);

		part->setState(RegularSwitch::State(0xff));
		QVERIFY_EXCEPTION_THROWN(part->commandState(), std::invalid_argument);
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
		QCOMPARE(part->switchState(), SwitchState::SWITCH_STATE_LEFT);

		part->setState(DoubleCrossSwitch::State::AD);
		QCOMPARE(part->state(), DoubleCrossSwitch::State::AD);
		QCOMPARE(part->switchState(), SwitchState::SWITCH_STATE_RIGHT);
		QCOMPARE(part->commandState(), Command::SETRGT);

		part->setState(DoubleCrossSwitch::State::BC);
		QCOMPARE(part->state(), DoubleCrossSwitch::State::BC);
		QCOMPARE(part->switchState(), SwitchState::SWITCH_STATE_RIGHT);
		QCOMPARE(part->commandState(), Command::SETRGT);

		part->setState(DoubleCrossSwitch::State::BD);
		QCOMPARE(part->state(), DoubleCrossSwitch::State::BD);
		QCOMPARE(part->switchState(), SwitchState::SWITCH_STATE_LEFT);
		QCOMPARE(part->commandState(), Command::SETLFT);
	}
}

class TestPosition : public Position
{
	const QString text;

	friend class TestModel;

public:
	static const QString TEST_KEY;

	explicit TestPosition(const QString & input = "") : text(input)
	{
	}

	QString key() const override
	{
		return text;
	}

	void testParseValue(const TestPosition & other)
	{
		const QString value = other.value();

		parse(value);
	}

	void testParse(const QString & value)
	{
		parse(value);
	}

	static void reset()
	{
		counter = 0;
	}
};

const QString TestPosition::TEST_KEY = "test-key";

void TestModel::testDefaultPosition()
{
	TestPosition position(TestPosition::TEST_KEY);

	QCOMPARE(position.bending(), Bending::STRAIGHT);
	QCOMPARE(position.isInclined(), false);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.point(), QPoint(0, 0));
	QCOMPARE(position.key(), TestPosition::TEST_KEY);
}

void TestModel::testParsingPosition()
{
	TestPosition position;
	TestPosition copy;

	position.testParse("");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::STRAIGHT);
	QCOMPARE(position.isInclined(), false);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.point(), QPoint(0, 0));

	position.testParse("x,y,i");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::STRAIGHT);
	QCOMPARE(position.isInclined(), true);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.point(), QPoint(4, 0));

	position.testParse("2,3,xxxvvr");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::RIGHT);
	QCOMPARE(position.isInclined(), false);
	QCOMPARE(position.extension(), 3u);
	QCOMPARE(position.lines(), 2u);
	QCOMPARE(position.point(), QPoint(8, 12));

	position.testParse("4,5,ql");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::LEFT);
	QCOMPARE(position.isInclined(), false);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.point(), QPoint(17, 20));

	position.testParse("6,7,hrxyvi");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::RIGHT);
	QCOMPARE(position.isInclined(), true);
	QCOMPARE(position.extension(), 1u);
	QCOMPARE(position.lines(), 1u);
	QCOMPARE(position.point(), QPoint(26, 28));
}

void TestModel::testExtension()
{
	TestPosition position;
	TestPosition copy;

	QCOMPARE(Position::FRACTION, 4u);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 4);

	copy.testParseValue(position);
	QCOMPARE(copy, position);

	position.extend();
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.extension(), 1u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 5);

	position.extend(2);
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.extension(), 3u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 7);

	position.extend(-1);
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.extension(), 2u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 6);

	position.extend(-4);
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 4);
}

void TestModel::testPosition()
{
	TestPosition position1;
	TestPosition position2;
	TestPosition copy;

	copy.testParseValue(position1);
	QCOMPARE(copy, position1);
	QVERIFY(position1 == position2);
	QCOMPARE(position1.point(), QPoint(0, 0));
	QCOMPARE(position2.point(), QPoint(0, 0));

	position1.move(4, 7);
	copy.testParseValue(position1);
	QCOMPARE(copy, position1);
	QVERIFY(position1 != position2);
	QCOMPARE(position1.point(), QPoint(4, 7));

	position1.move(-1, -2);
	copy.testParseValue(position1);
	QCOMPARE(copy, position1);
	QVERIFY(position1 != position2);
	QCOMPARE(position1.point(), QPoint(3, 5));

	position1.setX(12);
	copy.testParseValue(position1);
	QCOMPARE(copy, position1);
	QVERIFY(position1 != position2);
	QCOMPARE(position1.point(), QPoint(12, 5));

	QVERIFY(position1 != position2);
	QCOMPARE(Position::less(&position1, &position2), false);
	QCOMPARE(Position::less(&position2, &position1), true);
	position2.move(12, 5);
	copy.testParseValue(position1);
	QCOMPARE(copy, position1);
	QVERIFY(position1 == position2);
	QCOMPARE(Position::less(&position1, &position2), false);
	QCOMPARE(Position::less(&position2, &position1), false);

	copy.testParseValue(position1);
	QCOMPARE(copy, position1);
	QVERIFY(position1 == position2);
	QCOMPARE(position1.isInclined(), false);

	position1.toggleInclination();
	copy.testParseValue(position1);
	QCOMPARE(copy, position1);
	QVERIFY(position1 != position2);
	QCOMPARE(position1.isInclined(), true);

	position1.toggleInclination();
	copy.testParseValue(position1);
	QCOMPARE(copy, position1);
	QVERIFY(position1 == position2);
	QCOMPARE(position1.isInclined(), false);

	QVERIFY(position1 == position2);
	QCOMPARE(position2.bending(), Bending::STRAIGHT);

	position2.setBending(Bending::LEFT);
	copy.testParseValue(position2);
	QCOMPARE(copy, position2);
	QVERIFY(position1 != position2);
	QCOMPARE(position2.bending(), Bending::LEFT);

	position2.setBending(Bending::RIGHT);
	copy.testParseValue(position2);
	QCOMPARE(copy, position2);
	QVERIFY(position1 != position2);
	QCOMPARE(position2.bending(), Bending::RIGHT);

	position2.setBending(Bending::STRAIGHT);
	copy.testParseValue(position2);
	QCOMPARE(copy, position2);
	QVERIFY(position1 == position2);
	QCOMPARE(position2.bending(), Bending::STRAIGHT);
}

void TestModel::testDevice()
{
	std::vector<Device *> devices;

	model->parts<Device>(devices);
	for (Device * device : devices)
	{
		MrwMessage message = device->command(PING);

		QCOMPARE(message.command(), PING);
		QCOMPARE(message.unitNo(), device->unitNo());
		QCOMPARE(message.sid(), device->controller()->id());

		QCOMPARE(model->deviceByUnitNo(device->unitNo()), device);
		QCOMPARE(model->controllerById(device->controller()->id()), device->controller());
	}
}

void TestModel::testSection(Region * region, Section * section)
{
	QVERIFY(section != nullptr);
	QVERIFY(section->valid());
	QVERIFY(section->unitNo() != 0);
	QVERIFY(section->controller() != nullptr);
	QVERIFY(section->key().size() > 0);
	QVERIFY(section->unlockable());
	QCOMPARE(section->region(), region);

	SectionModule * module = section->module();

	QVERIFY2(module != nullptr, section->name().toStdString().c_str());

	const size_t rail_count = section->assemblyPartCount();
	for (unsigned r = 0; r < rail_count; r++)
	{
		AssemblyPart * part = section->assemblyPart(r);

		testAssemblyPart(section, part);
	}
	section->setOccupation(true);
	QVERIFY(section->unlockable());
	section->setState(SectionState::TOUR);
	QVERIFY(!section->unlockable());
	section->free();
	QVERIFY(section->unlockable());

	QVERIFY_EXCEPTION_THROWN(section->assemblyPart(rail_count), std::out_of_range);
}

void TestModel::testAssemblyPart(Section * section, AssemblyPart * part)
{
	QVERIFY(part != nullptr);
	QCOMPARE(part->section(), section);

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
		QVERIFY2(signal->key().contains(signal->partName()), name.c_str());
	}

	if (rail != nullptr)
	{
		QVERIFY2(signal == nullptr, name.c_str());
		QVERIFY2(rail->valid(), name.c_str());
		QVERIFY2(rail->key().contains(rail->partName()), name.c_str());

		QVERIFY(section->unlockable());
		rail->reserve();
		QVERIFY(!section->unlockable());
		rail->reserve(false);
		QVERIFY(section->unlockable());
	}

	if (device != nullptr)
	{
		QVERIFY2(device->unitNo() != 0, name.c_str());
		QVERIFY2(device->controller() != nullptr, name.c_str());
		QCOMPARE(device->lock(), Device::LockState::UNLOCKED);
	}
}

void TestModel::init()
{
	TestPosition::reset();
}
