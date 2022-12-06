//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTROUTING_H
#define MRW_TEST_TESTROUTING_H

#include <vector>
#include <model/route.h>

#include <testmodelbase.h>

namespace mrw::test
{
	class TestRouting : public TestModelBase
	{
		Q_OBJECT

	private:
		std::vector<mrw::model::RailPart *> parts;

	public:
		TestRouting();

	private slots:
		void initTestCase();
		void init();

		void testSimple();
		void testCounterPart();
		void testCrossBlocked();

	private:
		bool verify(const model::Route & route) const;
		bool verify(std::initializer_list<const model::Route *> routes) const;
		bool empty() const;
	};
}

#endif
