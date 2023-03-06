//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTFLANKSWITCH_H
#define MRW_TEST_TESTFLANKSWITCH_H

#include <QObject>

#include "testmodel.h"

namespace mrw::test
{
	class TestFlankSwitch : public TestModel
	{
		Q_OBJECT

	public:
		TestFlankSwitch();

	private slots:
		void init();

		void testFlankProtectionLeft();
		void testFlankProtectionMid();
		void testFlankProtectionRight();
		void testFlankProtectionDcs();
		void testFlankProtectionRsLeft();
		void testFlankProtectionRsRight();

		void testFlankProtectionDoubleU1();
		void testFlankProtectionDoubleU2();
		void testFlankProtectionDoubleU3();
		void testFlankProtectionDoubleURoute();
		void testFlankProtectionRhombus();
		void testFlankProtectionRhombusRouteLR();
		void testFlankProtectionRhombusRouteRL();
		void testFlankProtectionFar();
	};
}

#endif
