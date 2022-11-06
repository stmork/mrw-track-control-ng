//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTLIGHT_H
#define MRW_TEST_TESTLIGHT_H

#include <QObject>

#include "testmodel.h"

namespace mrw::test
{
	class TestLight : public TestModel
	{
		Q_OBJECT

	public:
		TestLight();

	private slots:
		void testLight();
		void testLightModule();
	};

}

#endif
