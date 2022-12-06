//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTROUTING_H
#define MRW_TEST_TESTROUTING_H

#include <model/route.h>

#include <testmodelbase.h>

namespace mrw::test
{
	class TestRouting : public TestModelBase
	{
		Q_OBJECT

	public:
		TestRouting();

	private slots:
		void init();

		void testSimple();
		void testCounterPart();
		void testCrossBlocked();
		void testExtension();

	private:
		bool verify(const model::Route & route) const;
		bool verify(std::initializer_list<const model::Route *> routes) const;
		bool empty() const;
	};
}

#endif
