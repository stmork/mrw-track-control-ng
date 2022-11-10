//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTNUMBERING_H
#define MRW_TEST_TESTNUMBERING_H

#include "testmodelbase.h"

namespace mrw::test
{
	class TestNumbering : public TestModelBase
	{
		Q_OBJECT

	public:
		TestNumbering();

	private slots:
		void testInvalid();
		void testMissingModule();
		void testDirectionFail();
		void testEmptyFail();
		void testToManyLights();
		void testToManyInductors();
		void testPorts();
	};
}

#endif
