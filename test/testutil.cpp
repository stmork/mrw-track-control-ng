//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <unistd.h>

#include <QTest>

#include <util/method.h>
#include <util/singleton.h>

#include "testutil.h"

using namespace mrw::test;
using namespace mrw::util;

const ConstantEnumerator<unsigned> TestUtil::map
{
	CONSTANT(EINVAL),
	CONSTANT(EBUSY)
};

class SingletonImpl : public mrw::util::Singleton<SingletonImpl>
{
	SingletonImpl()
	{
	}

	friend class Singleton<SingletonImpl>;

public:
	bool demo()
	{
		return true;
	}
};

TestUtil::TestUtil(QObject * parent) : QObject(parent)
{
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
