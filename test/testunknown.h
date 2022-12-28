//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTUNKNOWN_H
#define MRW_TEST_TESTUNKNOWN_H

#include "testmodelbase.h"

namespace mrw::test
{
	class TestUnknown : public TestModelBase
	{
		Q_OBJECT

	public:
		TestUnknown();

	private slots:
		void testInvalid();
		void testController();
		void testModule();
		void testMuxConnection();
	};
}

#endif
