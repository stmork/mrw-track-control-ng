//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <iostream>

#include <unistd.h>

#include <QTest>
#include <QSignalSpy>

#include <util/appsupport.h>
#include <util/method.h>
#include <util/properties.h>
#include <util/settings.h>
#include <util/singleton.h>
#include <util/dumphandler.h>
#include <util/termhandler.h>
#include <util/stringutil.h>
#include <util/clockservice.h>
#include <util/random.h>
#include <util/batchparticipant.h>
#include <util/globalbatch.h>
#include <util/self.h>
#include <util/hexline.h>
#include <util/cleanvector.h>

#include "testutil.h"

using namespace mrw::test;
using namespace mrw::util;

#if (QT_VERSION < QT_VERSION_CHECK(6, 3, 0))
#	define MRW_THROWS_EXCEPTION(condition, exception) QVERIFY_EXCEPTION_THROWN(condition, exception);
#else
#	define MRW_THROWS_EXCEPTION(condition, exception) QVERIFY_THROWS_EXCEPTION(exception, condition);
#endif

const ConstantEnumerator<int> TestUtil::int_map
{
	CONSTANT(EINVAL),
	CONSTANT(EBUSY)
};

const ConstantEnumerator<EnumTest> TestUtil::enum_map
{
	CONSTANT(EnumTest::ENUM1),
	CONSTANT(EnumTest::ENUM2)
};

/*************************************************************************
**                                                                      **
**       Test classes derived from abstract classes                     **
**                                                                      **
*************************************************************************/

class TestString : public String
{
	const QString string;

public:
	explicit TestString(const char * input) : string(input)
	{
	}

	inline QString toString() const override
	{
		return string;
	}
};

class TestParticipant : public BatchParticipant
{
	const QString label;

public:
	explicit TestParticipant(const QString & new_name) : label(new_name)
	{
	}

	virtual const QString & name() const noexcept override
	{
		return label;
	}
};

/*************************************************************************
**                                                                      **
**       Test case implementation                                       **
**                                                                      **
*************************************************************************/

class SingletonImpl : public Singleton<SingletonImpl>
{
	SingletonImpl() = default;

	friend class Singleton<SingletonImpl>;

public:
	bool demo()
	{
		return true;
	}
};

TestUtil::TestUtil(QObject * parent) : QObject(parent)
{
	Duration::pattern();
}

void TestUtil::init()
{
	GlobalBatch::instance().reset();
}

void TestUtil::testSettings()
{
	Settings      settings("model");
	SettingsGroup group(&settings, "model");

	const QString & filename = settings.value("filename", "Example").toString();

	QVERIFY(filename.size() > 0);
}

void TestUtil::testStringConcat()
{
	QString     left("a");
	TestString  right("b");
	QString     result = left + right;

	QCOMPARE(result, "ab");
}

void TestUtil::testStringOut()
{
	QString     bold = String::bold("fat!");
	QString     red  = String::red("red!");
	TestString  out("out!");

	QVERIFY(bold.size() > 4);
	QVERIFY(red.size() > 4);

	qDebug() << out;
	std::cout << out << std::endl;

	QCOMPARE(out.toString(), "out!");
	QCOMPARE(out, QString("out!"));
}

void TestUtil::testMethod()
{
	__METHOD__;
	const int value = 123456;

	usleep(value);
	QCOMPARE(Method::always<int>(&value),  true);
	QCOMPARE(Method::never<int>(&value),  false);
	QCOMPARE(Method::always<int>(nullptr), true);
	QCOMPARE(Method::never<int>(nullptr), false);
}

void TestUtil::testConstantEnumerator()
{
	QCOMPARE(int_map.at(EBUSY),   "EBUSY");
	QCOMPARE(int_map.get(EBUSY),  "EBUSY");
	QCOMPARE(int_map.at(EINVAL),  "EINVAL");
	QCOMPARE(int_map.get(EINVAL), "EINVAL");
	QCOMPARE(int_map.get(EAGAIN).left(2), "0x");

	QCOMPARE(int_map.findKey("EINVAL")->first, EINVAL);
	QCOMPARE(int_map.findKey("XYZ"), int_map.end());

	MRW_THROWS_EXCEPTION(int_map.at(EAGAIN), std::out_of_range);

	QCOMPARE(enum_map.at(EnumTest::ENUM1), "ENUM1");
	QCOMPARE(enum_map.get(EnumTest::ENUM1), "ENUM1");
	QCOMPARE(enum_map.at(EnumTest::ENUM2), "ENUM2");
	QCOMPARE(enum_map.get(EnumTest::ENUM2), "ENUM2");
	QCOMPARE(enum_map.findKey("XYZ"), enum_map.end());
}

void TestUtil::testSingleton()
{
	SingletonImpl & singleton(SingletonImpl::instance());

	QVERIFY(singleton.demo());
}

void TestUtil::testProperties()
{
	QString    filename = QFile::exists("test.properties") ? "test.properties" : "test/test.properties";
	Properties props(filename);

	QCOMPARE(props.size(), 5u);

	QCOMPARE(props.at("AvalidProp"), "xyvc_,");
	QCOMPARE(props.at("Another11Prop"), "ccc");
	QCOMPARE(props.at("Special"), "a.-_,");
	QCOMPARE(props.at("ss1"), "2");
	QCOMPARE(props.at("empty"), "");

	MRW_THROWS_EXCEPTION(props.at("11ss"), std::out_of_range);
	MRW_THROWS_EXCEPTION(props.at("1"),    std::out_of_range);

	QCOMPARE(props.lookup("AvalidProp"), "xyvc_,");
	QCOMPARE(props.lookup("Another11Prop"), "ccc");
	QCOMPARE(props.lookup("Special"), "a.-_,");
	QCOMPARE(props.lookup("ss1"), "2");
	QCOMPARE(props.lookup("empty"), "");
	QCOMPARE(props.lookup("11ss"), Properties::EMPTY);
	QCOMPARE(props.lookup("1"),    Properties::EMPTY);
}

void TestUtil::testClockService()
{
	QSignalSpy spy_1Hz(&ClockService::instance(), &ClockService::Hz1);
	QSignalSpy spy_2Hz(&ClockService::instance(), &ClockService::Hz2);
	QSignalSpy spy_4Hz(&ClockService::instance(), &ClockService::Hz4);
	QSignalSpy spy_8Hz(&ClockService::instance(), &ClockService::Hz8);

	QTest::qWait(2100);

	QVERIFY(spy_1Hz.count() >=  1);
	QVERIFY(spy_2Hz.count() >=  3);
	QVERIFY(spy_4Hz.count() >=  7);
	QVERIFY(spy_8Hz.count() >= 15);
}

void TestUtil::testRandom()
{
	static constexpr unsigned TEST_RAND_MIN      =      1;
	static constexpr unsigned TEST_RAND_MAX      =      6;
	static constexpr int      TEST_RAND_DICE_MAX =      7;
	static constexpr unsigned TEST_RAND_LOOP     = 100000;

	std::uniform_int_distribution<unsigned> dist(TEST_RAND_MIN, TEST_RAND_MAX);
	double mt_sum = 0;
	int    dice   = 0;

	for (unsigned i = 0; i < TEST_RAND_LOOP; i++)
	{
		const int r = Random::random<int>(TEST_RAND_DICE_MAX);

		QVERIFY(r >= 0);
		QVERIFY(r <= TEST_RAND_DICE_MAX);
		mt_sum += dist(Random::instance().engine());
		dice   += r;
	}

	const double avrg1 = 0.5 * (TEST_RAND_MIN + TEST_RAND_MAX) - mt_sum / TEST_RAND_LOOP;
	const int    avrg2 = dice / TEST_RAND_LOOP;

	QVERIFY(std::abs(avrg1) <= 0.02);
	QCOMPARE(avrg2, TEST_RAND_DICE_MAX / 2);
}

void TestUtil::testDumpHandler()
{
	size_t       count = 0;
	TermHandler  term_handler;
	DumpHandler  dump_handler( [&] ()
	{
		count++;
		qInfo("Called");
	});

	QCOMPARE(count, 0u);

#ifdef QTCREATOR
	return;
#endif

	raise(SIGQUIT);
	// cppcheck-suppress "unreachableCode"
	QCOMPARE(count, 1u);

	raise(SIGQUIT);
	// cppcheck-suppress "unreachableCode"
	QCOMPARE(count, 2u);
}

void TestUtil::testEmptyBatch()
{
	QSignalSpy      completed(&GlobalBatch::instance(), &GlobalBatch::completed);
	TestParticipant part_a("A");
	TestParticipant part_b("B");

	QVERIFY( GlobalBatch::instance().isCompleted());
	QVERIFY(!GlobalBatch::instance().contains(&part_a));
	QVERIFY(!GlobalBatch::instance().contains(&part_b));
	QCOMPARE(completed.count(), 0);

	GlobalBatch::instance().tryComplete();
	QCOMPARE(completed.count(), 1);
	QCOMPARE(part_a.batch(), &GlobalBatch::instance());
	QCOMPARE(part_b.batch(), &GlobalBatch::instance());
}

void TestUtil::testUseBatch()
{
	QSignalSpy      completed(&GlobalBatch::instance(), &GlobalBatch::completed);
	TestParticipant part_a("A");
	TestParticipant part_b("B");

	QVERIFY(part_a.increase());
	QVERIFY(!GlobalBatch::instance().isCompleted());
	QVERIFY( GlobalBatch::instance().contains(&part_a));
	QVERIFY(!GlobalBatch::instance().contains(&part_b));
	GlobalBatch::instance().tryComplete();
	QCOMPARE(completed.count(), 0);

	QVERIFY(part_b.increase());
	QVERIFY(!GlobalBatch::instance().isCompleted());
	QVERIFY( GlobalBatch::instance().contains(&part_a));
	QVERIFY( GlobalBatch::instance().contains(&part_b));
	GlobalBatch::instance().tryComplete();
	QCOMPARE(completed.count(), 0);

	GlobalBatch::instance().tryComplete();
	QCOMPARE(completed.count(), 0);

	QVERIFY(part_a.decrease());
	QVERIFY(!GlobalBatch::instance().isCompleted());
	QVERIFY(!GlobalBatch::instance().contains(&part_a));
	QVERIFY( GlobalBatch::instance().contains(&part_b));
	GlobalBatch::instance().tryComplete();
	QCOMPARE(completed.count(), 0);

	QVERIFY(part_b.decrease());
	QVERIFY( GlobalBatch::instance().isCompleted());
	QVERIFY(!GlobalBatch::instance().contains(&part_a));
	QVERIFY(!GlobalBatch::instance().contains(&part_b));
	QCOMPARE(completed.count(), 1);

	GlobalBatch::instance().tryComplete();
	QCOMPARE(completed.count(), 2);
}

void TestUtil::testResetBatch()
{
	QSignalSpy      completed(&GlobalBatch::instance(), &GlobalBatch::completed);
	std::vector<TestParticipant> participants;

	const size_t max = 100;
	participants.reserve(max);
	for (size_t i = 0; i < max; i++)
	{
		participants.emplace_back(TestParticipant(QString::asprintf("%04zu", i)));
	}

	QVERIFY( GlobalBatch::instance().isCompleted());
	for (TestParticipant & participant : participants)
	{
		QVERIFY(!GlobalBatch::instance().contains(&participant));
	}
	QCOMPARE(completed.count(), 0);

	for (TestParticipant & participant : participants)
	{
		QVERIFY(participant.increase());
	}
	QVERIFY(!GlobalBatch::instance().isCompleted());
	for (TestParticipant & participant : participants)
	{
		QVERIFY( GlobalBatch::instance().contains(&participant));
	}

	GlobalBatch::instance().reset();
	QVERIFY( GlobalBatch::instance().isCompleted());
	for (TestParticipant & participant : participants)
	{
		QVERIFY(!GlobalBatch::instance().contains(&participant));
	}
}

void TestUtil::testDoubleUseBatch()
{
	TestParticipant part_a("A");
	TestParticipant part_b("B");

	QVERIFY( part_a.increase());
	QVERIFY(!GlobalBatch::instance().isCompleted());
	QVERIFY( GlobalBatch::instance().contains(&part_a));
	QVERIFY(!GlobalBatch::instance().contains(&part_b));

	QVERIFY(!part_a.increase());
	QVERIFY(!GlobalBatch::instance().isCompleted());
	QVERIFY( GlobalBatch::instance().contains(&part_a));
	QVERIFY(!GlobalBatch::instance().contains(&part_b));

	QVERIFY(!part_b.decrease());
	QVERIFY(!GlobalBatch::instance().isCompleted());
	QVERIFY( GlobalBatch::instance().contains(&part_a));
	QVERIFY(!GlobalBatch::instance().contains(&part_b));

	QVERIFY(part_a.decrease());
	QVERIFY( GlobalBatch::instance().isCompleted());
	QVERIFY(!GlobalBatch::instance().contains(&part_a));
	QVERIFY(!GlobalBatch::instance().contains(&part_b));
}

void TestUtil::testDumpBatch()
{
	TestParticipant part_a("A");
	TestParticipant part_b("B");

	QVERIFY( part_a.increase());
	QVERIFY( part_b.increase());
	GlobalBatch::instance().dump();
}

void TestUtil::testCustomBatch()
{
	TestBatch       batch;
	TestParticipant part_a("A");
	TestParticipant part_b("B");

	part_a.setBatch(&batch);
	part_b.setBatch(&batch);

	QCOMPARE(part_a.batch(), &batch);
	QCOMPARE(part_b.batch(), &batch);

	part_a.setBatch(nullptr);
	part_b.setBatch(nullptr);
	QCOMPARE(part_a.batch(), &GlobalBatch::instance());
	QCOMPARE(part_b.batch(), &GlobalBatch::instance());
}

void TestUtil::testUnsetCustomBatch()
{
	std::vector<TestParticipant> participants;

	const size_t max = 100;
	participants.reserve(max);
	for (size_t i = 0; i < max; i++)
	{
		participants.emplace_back(TestParticipant(QString::asprintf("%04zu", i)));
	}

	{
		TestBatch batch;

		for (TestParticipant & participant : participants)
		{
			participant.setBatch(&batch);
			QCOMPARE(participant.batch(), &batch);
			QVERIFY( participant.increase());
			QVERIFY(!GlobalBatch::instance().contains(&participant));
			QVERIFY(batch.contains(&participant));
		}
	}

	for (TestParticipant & participant : participants)
	{
		QVERIFY(!GlobalBatch::instance().contains(&participant));
		QCOMPARE(participant.batch(), &GlobalBatch::instance());
	}
}

void TestUtil::testResetCustomBatch()
{
	TestBatch                    batch;
	QSignalSpy                   completed(&batch, &TestBatch::completed);
	std::vector<TestParticipant> participants;

	const size_t max = 100;
	participants.reserve(max);
	for (size_t i = 0; i < max; i++)
	{
		participants.emplace_back(TestParticipant(QString::asprintf("%04zu", i)));
	}

	QVERIFY(batch.isCompleted());
	for (TestParticipant & participant : participants)
	{
		QVERIFY(!batch.contains(&participant));
		participant.setBatch(&batch);
		QVERIFY(!GlobalBatch::instance().contains(&participant));
		QVERIFY(!batch.contains(&participant));
	}
	QCOMPARE(completed.count(), 0);

	for (TestParticipant & participant : participants)
	{
		QVERIFY(participant.increase());
		QVERIFY(!GlobalBatch::instance().contains(&participant));
		QVERIFY(batch.contains(&participant));
	}
	QVERIFY(GlobalBatch::instance().isCompleted());
	QVERIFY(!batch.isCompleted());

	batch.reset();
	QVERIFY(GlobalBatch::instance().isCompleted());
	QVERIFY(batch.isCompleted());
	for (TestParticipant & participant : participants)
	{
		QVERIFY(!GlobalBatch::instance().contains(&participant));
		QVERIFY(!batch.contains(&participant));
	}
}

void TestUtil::testDifferentBatch()
{
	TestBatch       batch;
	TestParticipant part_a("A");

	QVERIFY(part_a.increase());
	part_a.setBatch(&batch);
	QVERIFY(part_a.increase());
}

void TestUtil::testSelfPointer()
{
	Self<TestBatch>              self;
	std::shared_ptr<TestBatch> & shared_pointer = self;

	QVERIFY(shared_pointer);
	QCOMPARE(shared_pointer.use_count(), 1);
}

void TestUtil::testHexLine()
{
	std::vector<uint8_t> buffer;

	// Header parse error
	MRW_THROWS_EXCEPTION(HexLine(""), std::invalid_argument);
	MRW_THROWS_EXCEPTION(HexLine(":xxxx"), std::invalid_argument);
	MRW_THROWS_EXCEPTION(HexLine(":0000000g"), std::invalid_argument);

	// Length format error.
	MRW_THROWS_EXCEPTION(HexLine(":01000000"), std::invalid_argument);

	// Data format error.
	MRW_THROWS_EXCEPTION(HexLine(":02123400xx00aa"), std::invalid_argument);

	// Checksum error.
	MRW_THROWS_EXCEPTION(HexLine(":02123400ff00aa"), std::invalid_argument);

	// Data line
	HexLine line(":02123400ff00b9");
	QCOMPARE(line.getAddress(), unsigned(0x1234));
	QVERIFY(line);
	line.append(buffer);
	QCOMPARE(buffer.size(), size_t(2));
	QCOMPARE(buffer[0], 0xff);
	QCOMPARE(buffer[1], 0x00);

	// EOF
	HexLine eof(":005678010e");
	QCOMPARE(eof.getAddress(), unsigned(0x5678));
	QVERIFY(!eof);
}

struct Container
{
	float a = 0.f;
};

void TestUtil::testCleanVector()
{
	Container * a = new Container();
	Container * b = new Container();
	CleanVector<Container> container;

	container.push_back(a);
	container.push_back(b);
	QCOMPARE(container.size(), size_t(2));
}

void TestUtil::testSharedVector()
{
	std::shared_ptr<Container> a = std::make_shared<Container>();
	std::shared_ptr<Container> b = std::make_shared<Container>();
	std::shared_ptr<Container> c(new Container());

	QCOMPARE(a.use_count(), 1);
	QCOMPARE(b.use_count(), 1);
	QCOMPARE(c.use_count(), 1);

	{
		SharedVector<Container> container;

		QCOMPARE(a.use_count(), 1);
		QCOMPARE(b.use_count(), 1);
		QCOMPARE(c.use_count(), 1);

		container.push_back(a);
		container.push_back(b);
		container.push_back(c);

		QCOMPARE(a.use_count(), 2);
		QCOMPARE(b.use_count(), 2);
		QCOMPARE(c.use_count(), 2);
	}
	QCOMPARE(a.use_count(), 1);
	QCOMPARE(b.use_count(), 1);
	QCOMPARE(c.use_count(), 1);
}

void TestUtil::testBlanktime()
{
	const auto blanktime = AppSupport::instance().blanktime();

	QVERIFY(blanktime >= 300); // 5 minutes
	QVERIFY(blanktime <= 21600); // 6 hours
}

void TestUtil::testHostname()
{
	const auto hostname = AppSupport::instance().hostname();

	QVERIFY(!hostname.isEmpty());
	QVERIFY(!hostname.contains("."));
}
