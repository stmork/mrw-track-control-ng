//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTUTIL_H
#define MRW_TEST_TESTUTIL_H

#include <QObject>

#include <util/constantenumerator.h>

namespace mrw::test
{
	class TestUtil : public QObject
	{
		Q_OBJECT

	private:
		static const mrw::util::ConstantEnumerator<int> map;

	public:
		explicit TestUtil(QObject * parent = nullptr);

	private slots:
		void testSettings();
		void testStringConcat();
		void testStringOut();
		void testMethod();
		void testConstantEnumerator();
		void testSingleton();
		void testProperties();
		void testClockService();
		void testRandom();
	};
}

#endif
