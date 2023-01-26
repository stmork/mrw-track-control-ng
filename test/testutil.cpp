//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <iostream>

#include <unistd.h>

#include <QTest>
#include <QSignalSpy>

#include <util/method.h>
#include <util/properties.h>
#include <util/settings.h>
#include <util/singleton.h>
#include <util/dumphandler.h>
#include <util/termhandler.h>
#include <util/stringutil.h>
#include <util/clockservice.h>
#include <util/random.h>

#include "testutil.h"

using namespace mrw::test;
using namespace mrw::util;

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

const ConstantEnumerator<int> TestUtil::map
{
	CONSTANT(EINVAL),
	CONSTANT(EBUSY)
};

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
	Method::pattern();
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
	std::clog << out;
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
	QCOMPARE(map.at(EBUSY),   "EBUSY");
	QCOMPARE(map.get(EBUSY),  "EBUSY");
	QCOMPARE(map.at(EINVAL),  "EINVAL");
	QCOMPARE(map.get(EINVAL), "EINVAL");
	QCOMPARE(map.get(EAGAIN), "0x0B");

	QCOMPARE(map.findKey("EINVAL")->first, EINVAL);
	QCOMPARE(map.findKey("XYZ"), map.end());

	QVERIFY_EXCEPTION_THROWN(map.at(EAGAIN), std::out_of_range);
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
	QVERIFY_EXCEPTION_THROWN(props.at("11ss"), std::out_of_range);
	QVERIFY_EXCEPTION_THROWN(props.at("1"),    std::out_of_range);

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

	QCOMPARE(spy_1Hz.count(),  2);
	QCOMPARE(spy_2Hz.count(),  4);
	QCOMPARE(spy_4Hz.count(),  8);
	QCOMPARE(spy_8Hz.count(), 16);
}

void TestUtil::testRandom()
{
	static const unsigned TEST_RAND_MIN  = 1;
	static const unsigned TEST_RAND_MAX  = 6;
	static const unsigned TEST_RAND_LOOP = 100000;

	std::uniform_int_distribution<unsigned> dist(TEST_RAND_MIN, TEST_RAND_MAX);
	double mt_sum = 0;
	int    dice   = 0;

	for (unsigned i = 0; i < TEST_RAND_LOOP; i++)
	{
		mt_sum += dist(Random::instance().engine());
		dice   += Random::random(7);
	}

	const double diff = 0.5 * (TEST_RAND_MIN + TEST_RAND_MAX) - mt_sum / TEST_RAND_LOOP;

	QVERIFY(std::abs(diff) <= 0.02);
	QCOMPARE(dice / TEST_RAND_LOOP, 3u);
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

	raise(SIGQUIT);
	QCOMPARE(count, 1u);

	raise(SIGQUIT);
	QCOMPARE(count, 2u);
}
