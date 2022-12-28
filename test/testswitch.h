//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTSWITCH_H
#define MRW_TEST_TESTSWITCH_H

#include <QObject>

#include "testmodel.h"

namespace mrw::test
{
	class TestSwitch : public TestModel
	{
		Q_OBJECT

	public:
		TestSwitch();

	private slots:
		void testSwitch();
		void testSwitchModule();
	};

}

#endif
