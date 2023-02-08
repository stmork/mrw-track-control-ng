//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QTest>
#include <QStringLiteral>

#include <can/mrwmessage.h>
#include <model/switchmodulereference.h>
#include <model/controller.h>
#include <model/lightsignal.h>
#include <model/formsignal.h>
#include <model/railpart.h>
#include <model/regularswitch.h>
#include <model/doublecrossswitch.h>
#include <model/switchmodule.h>
#include <model/profilelight.h>

#include "testmodel.h"

using namespace mrw::can;
using namespace mrw::test;
using namespace mrw::model;

using Bending    = Position::Bending;
using LockState  = Device::LockState;
using SignalType = Signal::SignalType;

/*************************************************************************
**                                                                      **
**       Test position implementation                                   **
**                                                                      **
*************************************************************************/

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

/*************************************************************************
**                                                                      **
**      Model test cases                                                **
**                                                                      **
*************************************************************************/

TestModel::TestModel(const char * modelname, QObject * parent) :
	TestModelBase(modelname, parent)
{
}

void TestModel::init()
{
	TestPosition::reset();

	std::vector<Device *> devices;

	model->parts<Device>(devices);
	for (Device * device : devices)
	{
		device->setLock(LockState::UNLOCKED);
	}
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
		Controller        *       controller = model->controller(c);
		std::vector<MrwMessage>   messages;

		QVERIFY(controller != nullptr);
		QVERIFY(controller->valid());
		QVERIFY(controller->id() != 0);

		controller->configure(messages);
		QCOMPARE(messages.size() > 0, controller->valid());
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
		QString  name   = region->name();

		region->direction();
		QVERIFY(region != nullptr);
		QVERIFY(region->key().contains(name.replace(" ", "")));
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
		QCOMPARE(part->isBranch(), !part->isRightHanded());

		part->setState(RegularSwitch::State::AC);
		QCOMPARE(part->state(), RegularSwitch::State::AC);
		QCOMPARE(part->switchState(), SwitchState::SWITCH_STATE_RIGHT);
		QCOMPARE(part->commandState(), Command::SETRGT);
		QCOMPARE(part->isBranch(), part->isRightHanded());

		part->setState(RegularSwitch::State(0xff));
		QVERIFY_EXCEPTION_THROWN(part->commandState(), std::invalid_argument);

		for (RailPart * left : part->advance(true))
		{
			for (RailPart * right : part->advance(false))
			{
				part->setState(left, right);
				QVERIFY(
					(part->state() == RegularSwitch::State::AB) ||
					(part->state() == RegularSwitch::State::AC));

				part->setState(right, left);
				QVERIFY(
					(part->state() == RegularSwitch::State::AB) ||
					(part->state() == RegularSwitch::State::AC));

				QVERIFY_EXCEPTION_THROWN(part->setState(right, nullptr), std::invalid_argument);
				QVERIFY_EXCEPTION_THROWN(part->setState(left, nullptr),  std::invalid_argument);

				QVERIFY_EXCEPTION_THROWN(part->setState(nullptr, right), std::invalid_argument);
				QVERIFY_EXCEPTION_THROWN(part->setState(nullptr, left),  std::invalid_argument);

				QVERIFY_EXCEPTION_THROWN(part->setState(nullptr, nullptr), std::invalid_argument);
				QVERIFY_EXCEPTION_THROWN(part->setState(nullptr, nullptr), std::invalid_argument);
			}
		}
	}
}

void TestModel::testDoubleCrossSwitchStates()
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

		for (RailPart * left : part->advance(true))
		{
			for (RailPart * right : part->advance(false))
			{
				part->setState(left, right);
				QVERIFY(
					(part->state() == DoubleCrossSwitch::State::AC) ||
					(part->state() == DoubleCrossSwitch::State::AD) ||
					(part->state() == DoubleCrossSwitch::State::BC) ||
					(part->state() == DoubleCrossSwitch::State::BD));

				part->setState(right, left);
				QVERIFY(
					(part->state() == DoubleCrossSwitch::State::AC) ||
					(part->state() == DoubleCrossSwitch::State::AD) ||
					(part->state() == DoubleCrossSwitch::State::BC) ||
					(part->state() == DoubleCrossSwitch::State::BD));

				QVERIFY_EXCEPTION_THROWN(part->setState(right, nullptr), std::invalid_argument);
				QVERIFY_EXCEPTION_THROWN(part->setState(left, nullptr),  std::invalid_argument);

				QVERIFY_EXCEPTION_THROWN(part->setState(nullptr, right), std::invalid_argument);
				QVERIFY_EXCEPTION_THROWN(part->setState(nullptr, left),  std::invalid_argument);

				QVERIFY_EXCEPTION_THROWN(part->setState(nullptr, nullptr), std::invalid_argument);
				QVERIFY_EXCEPTION_THROWN(part->setState(nullptr, nullptr), std::invalid_argument);
			}
		}
	}
}

void TestModel::testSignals()
{
	std::vector<Signal *> all_signals;

	model->parts<Signal>(all_signals);
	for (Signal * signal : all_signals)
	{
		Section * section = signal->section();

		QVERIFY(section != nullptr);

		const std::vector<Signal *> & section_signals =
			section->getSignals(signal->direction());

		QVERIFY(!section_signals.empty());
	}
}

void TestModel::testDefaultPosition()
{
	TestPosition position(TestPosition::TEST_KEY);

	QCOMPARE(position.bending(), Bending::STRAIGHT);
	QCOMPARE(position.isInclined(), false);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.point(), QPoint(0, 0));
	QCOMPARE(position.key(), TestPosition::TEST_KEY);
	QCOMPARE(position.width(), 4);
	QCOMPARE(position.height(), 4);
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
	QCOMPARE(position.width(), 4);
	QCOMPARE(position.height(), 4);

	position.testParse("x,y,i");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::STRAIGHT);
	QCOMPARE(position.isInclined(), true);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.point(), QPoint(4, 0));
	QCOMPARE(position.width(), 4);
	QCOMPARE(position.height(), 4);

	position.testParse("2,3,xxxvvr");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::RIGHT);
	QCOMPARE(position.isInclined(), false);
	QCOMPARE(position.extension(), 3u);
	QCOMPARE(position.lines(), 2u);
	QCOMPARE(position.point(), QPoint(8, 12));
	QCOMPARE(position.width(), 7);
	QCOMPARE(position.height(), 12);

	position.testParse("4,5,ql");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::LEFT);
	QCOMPARE(position.isInclined(), false);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.point(), QPoint(17, 20));
	QCOMPARE(position.width(), 4);
	QCOMPARE(position.height(), 4);

	position.testParse("6,7,hrxyvi");
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.bending(), Bending::RIGHT);
	QCOMPARE(position.isInclined(), true);
	QCOMPARE(position.extension(), 1u);
	QCOMPARE(position.lines(), 1u);
	QCOMPARE(position.point(), QPoint(26, 28));
	QCOMPARE(position.width(), 5);
	QCOMPARE(position.height(), 8);
}

void TestModel::testExtension()
{
	TestPosition position;
	TestPosition copy;

	QCOMPARE(Position::FRACTION, 4u);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 4);
	QCOMPARE(position.height(), 4);

	copy.testParseValue(position);
	QCOMPARE(copy, position);

	position.extend();
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.extension(), 1u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 5);
	QCOMPARE(position.height(), 4);

	position.extend(2);
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.extension(), 3u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 7);
	QCOMPARE(position.height(), 4);

	position.extend(-1);
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.extension(), 2u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 6);
	QCOMPARE(position.height(), 4);

	position.extend(-4);
	copy.testParseValue(position);
	QCOMPARE(copy, position);
	QCOMPARE(position.extension(), 0u);
	QCOMPARE(position.lines(), 0u);
	QCOMPARE(position.width(), 4);
	QCOMPARE(position.height(), 4);
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

		QCOMPARE(model->deviceById(message.sid(), message.unitNo()), device);
		QCOMPARE(model->deviceById(device->controller()->id(), device->unitNo()), device);
		QCOMPARE(model->controllerById(device->controller()->id()), device->controller());
	}
}

void TestModel::testEnumerator()
{
	QCOMPARE(Signal::get(Signal::Symbol::OFF),  "OFF");
	QCOMPARE(Signal::get(Signal::Symbol(0xff)), "0xFF");

	QCOMPARE(Signal::get(Signal::SignalType::MAIN_SIGNAL), "MAIN_SIGNAL");
	QCOMPARE(Signal::get(Signal::SignalType(0xff)),        "0xFF");

	QCOMPARE(RegularSwitch::get(RegularSwitch::State::AB),   "State::AB");
	QCOMPARE(RegularSwitch::get(RegularSwitch::State::AC),   "State::AC");
	QCOMPARE(RegularSwitch::get(RegularSwitch::State(0xff)), "0xFF");

	QCOMPARE(DoubleCrossSwitch::get(DoubleCrossSwitch::State::AC), "State::AC");
	QCOMPARE(DoubleCrossSwitch::get(DoubleCrossSwitch::State(0xff)), "0xFF");

	QCOMPARE(Section::get(SectionState::FREE), "FREE");
	QCOMPARE(Section::get(SectionState(0xff)), "0xFF");
}

void TestModel::testLightSignalConfig()
{
	testLightSignalConfig(2, SignalType::MAIN_SIGNAL,       CFGML2);
	testLightSignalConfig(3, SignalType::MAIN_SIGNAL,       CFGML3);
	testLightSignalConfig(5, SignalType::MAIN_SHUNT_SIGNAL, CFGML4);
	testLightSignalConfig(2, SignalType::DISTANT_SIGNAL,    CFGPL2);
	testLightSignalConfig(4, SignalType::DISTANT_SIGNAL,    CFGPL3);
	testLightSignalConfig(2, SignalType::SHUNT_SIGNAL,      CFGSL2);
}

void TestModel::testFormSignalConfig()
{
	testFormSignalConfig(2, SignalType::DISTANT_SIGNAL,     CFGPF2);
	testFormSignalConfig(3, SignalType::DISTANT_SIGNAL,     CFGPF3);
	testFormSignalConfig(2, SignalType::MAIN_SIGNAL,        CFGMF2);
	testFormSignalConfig(3, SignalType::MAIN_SIGNAL,        CFGMF3);
	testFormSignalConfig(2, SignalType::SHUNT_SIGNAL,       CMD_ILLEGAL);
}

void TestModel::testSectionConfig()
{
	std::vector<Section *> sections;

	model->parts<Section>(sections);

	for (const Section * section : sections)
	{
		for (unsigned pin = 0; pin < 4; pin++)
		{
			const MrwMessage msg = section->configMsg(pin);

			QCOMPARE(msg.size(),    2u);
			QCOMPARE(msg.command(), CFGRAI);
			QCOMPARE(msg[0], pin);
			QCOMPARE(msg[1], 7 - pin);
		}
	}
}

void TestModel::testSwitchConfig()
{
	std::vector<AbstractSwitch *> switches;

	model->parts<AbstractSwitch>(switches, [](const AbstractSwitch * part)
	{
		return part->hasCutOff();
	});

	for (const AbstractSwitch * part : switches)
	{
		for (unsigned pin = 0; pin < 4; pin++)
		{
			const MrwMessage msg = part->configMsg(pin);

			QCOMPARE(msg.size(),    4u);
			QCOMPARE(msg.command(), CFGSWN);
			for (unsigned i = 0; i < 2; i++)
			{
				QCOMPARE(msg[i], i + pin);
				QCOMPARE(msg[i + 2], i + 8 + pin);
			}
		}
	}
}

void TestModel::testSimpleLightConfig()
{
	std::vector<Light *> lights;

	model->parts<Light>(lights, [](const Light * light)
	{
		// Filter out profile light to get only simple lights.
		return dynamic_cast<const ProfileLight *>(light) == nullptr;
	});

	for (const Light * light : lights)
	{
		QVERIFY(light->isUnlockable());
		for (unsigned pin = 0; pin < 10; pin++)
		{
			const MrwMessage msg = light->configMsg(pin);

			QCOMPARE(msg.size(),    2u);
			QCOMPARE(msg.command(), CFGLGT);
			QCOMPARE(msg[0], pin);
		}
	}
}

void TestModel::testProfileLightConfig()
{
	std::vector<ProfileLight *> lights;

	model->parts<ProfileLight>(lights);

	for (const Light * light : lights)
	{
		QVERIFY(light->isUnlockable());
		for (unsigned pin = 0; pin < 8; pin++)
		{
			const MrwMessage msg = light->configMsg(pin);

			QCOMPARE(msg.size(),    3u);
			QCOMPARE(msg.command(), CFGLGT);
			QCOMPARE(msg[0], pin);
		}
	}
}

void TestModel::testSection(Region * region, Section * section)
{
	QVERIFY(section != nullptr);
	QVERIFY(section->valid());
	QVERIFY(section->unitNo() != 0);
	QVERIFY(section->controller() != nullptr);
	QVERIFY(section->key().size() > 0);
	QVERIFY(section->isUnlockable());
	QCOMPARE(section->region(), region);

	section->enable();
	QVERIFY(section->enabled());

	section->enable(false);
	QVERIFY(!section->enabled());

	section->disable(false);
	QVERIFY(section->enabled());

	section->disable();
	QVERIFY(!section->enabled());

	SectionModule * module = section->module();

	QVERIFY2(module != nullptr, section->name().toStdString().c_str());

	const size_t rail_count = section->assemblyPartCount();
	for (unsigned r = 0; r < rail_count; r++)
	{
		AssemblyPart * part = section->assemblyPart(r);

		testAssemblyPart(section, part);
	}
	section->setOccupation(true);
	QVERIFY(section->isUnlockable());
	QVERIFY(section->isFree());
	QCOMPARE(section->state(), SectionState::OCCUPIED);
	section->setState(SectionState::TOUR);
	QVERIFY(!section->isUnlockable());
	section->free();
	QVERIFY(section->isUnlockable());
	QVERIFY(section->isFree());

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
	QCOMPARE(part->region(), section->region());

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

		QCOMPARE(signal->state(), SignalAspect::SIGNAL_OFF);
		signal->setState(SignalAspect::SIGNAL_TST);
		QCOMPARE(signal->state(), SignalAspect::SIGNAL_TST);
	}

	if (rail != nullptr)
	{
		QVERIFY2(signal == nullptr, name.c_str());
		QVERIFY2(rail->valid(), name.c_str());
		QVERIFY2(rail->key().contains(rail->partName()), name.c_str());

		QVERIFY(section->isUnlockable());
		rail->reserve();
		QVERIFY(!section->isUnlockable());
		rail->reserve(false);
		QVERIFY(section->isUnlockable());
	}

	if (device != nullptr)
	{
		QVERIFY2(device->unitNo() != 0, name.c_str());
		QVERIFY2(device->controller() != nullptr, name.c_str());
		QCOMPARE(device->lock(), LockState::UNLOCKED);

		device->setLock(LockState::LOCKED);
		QCOMPARE(device->lock(), LockState::LOCKED);

		device->setLock(LockState::PENDING);
		QCOMPARE(device->lock(), LockState::PENDING);

		device->setLock(LockState::FAIL);
		QCOMPARE(device->lock(), LockState::FAIL);

		device->setLock(LockState::UNLOCKED);
		QCOMPARE(device->lock(), LockState::UNLOCKED);
	}
}

void TestModel::testLightSignalConfig(
	const unsigned           pins,
	const Signal::SignalType type,
	const Command            command)
{
	std::vector<LightSignal *> light_signals;

	model->parts<LightSignal>(light_signals, [&](const LightSignal * signal)
	{
		return (signal->usedPins() == pins) && (signal->type() == type);
	});

	for (const LightSignal * signal : light_signals)
	{
		for (unsigned pin = 0; pin < 10; pin++)
		{
			const MrwMessage msg = signal->configMsg(pin);

			QCOMPARE(msg.size(),    pins);
			QCOMPARE(msg.command(), command);
			for (unsigned p = 0; p < pins; p++)
			{
				QCOMPARE(msg[p], p + pin);
			}
		}
	}
}

void TestModel::testFormSignalConfig(
	const unsigned           inductors,
	const Signal::SignalType type,
	const Command            command)
{
	std::vector<FormSignal *> form_signals;

	model->parts<FormSignal>(form_signals, [&](const FormSignal * signal)
	{
		return (signal->inductors() == inductors) && (signal->type() == type);
	});

	for (const FormSignal * signal : form_signals)
	{
		for (unsigned pin = 0; pin < 10; pin++)
		{
			const MrwMessage msg = signal->configMsg(pin);

			QCOMPARE(msg.size(),    inductors);
			QCOMPARE(msg.command(), command);
			for (unsigned i = 0; i < inductors; i++)
			{
				QCOMPARE(msg[i], i + pin);
			}
		}
	}
}
