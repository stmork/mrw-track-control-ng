//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

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
	TermHandler      term_handler( { SIGTERM, SIGINT } );
	DumpHandler      dump_handler( [] ()
	{
		qInfo("Called");
	});
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

void TestUtil::testMethod()
{
	__METHOD__;

	usleep(123456);
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
