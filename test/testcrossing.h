//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTCROSSING_H
#define MRW_TEST_TESTCROSSING_H

#include "testmodelbase.h"

namespace mrw::test
{
	class TestCrossing : public TestModelBase
	{
		Q_OBJECT

	public:
		TestCrossing();

	private slots:
		void testSimple();
		void testDown();
		void testUpper();
		void testLower();
		void testBoth();
	};
}

#endif
