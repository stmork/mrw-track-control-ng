//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTUTIL_H
#define MRW_TEST_TESTUTIL_H

#include <QObject>

#include <util/constantenumerator.h>
#include <util/batch.h>

namespace mrw::test
{
	enum class EnumTest
	{
		ENUM1,
		ENUM2
	};

	class TestBatch : public QObject, public mrw::util::Batch
	{
		Q_OBJECT

	public:
		TestBatch() : QObject(nullptr), Batch()
		{
		}

		virtual ~TestBatch() = default;

	signals:
		void completed() override;
	};

	class TestUtil : public QObject
	{
		Q_OBJECT

	private:
		static const mrw::util::ConstantEnumerator<int>      int_map;
		static const mrw::util::ConstantEnumerator<EnumTest> enum_map;

	public:
		explicit TestUtil(QObject * parent = nullptr);

	private slots:
		void init();

		void testSettings();
		void testStringConcat();
		void testStringOut();
		void testMethod();
		void testConstantEnumerator();
		void testSingleton();
		void testProperties();
		void testClockService();
		void testRandom();
		void testDumpHandler();
		void testEmptyBatch();
		void testUseBatch();
		void testResetBatch();
		void testDoubleUseBatch();
		void testDumpBatch();
		void testCustomBatch();
		void testUnsetCustomBatch();
		void testResetCustomBatch();
		void testDifferentBatch();
		void testSelfPointer();
		void testHexLine();
		void testCleanVector();
		void testSharedVector();
		void testBlanktime();
		void testHostname();
	};
}

#endif
