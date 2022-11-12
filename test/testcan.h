//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTCAN_H
#define MRW_TEST_TESTCAN_H

#include <QObject>

#include <model/controller.h>
#include <model/device.h>

namespace mrw::test
{
	class TestCan : public QObject
	{
		Q_OBJECT

		static constexpr mrw::model::ControllerId TEST_CTRL_ID = 0x1234 >> 2;
		static constexpr mrw::model::UnitNo       TEST_UNIT_NO = 0x5678;
		static constexpr quint32                  TEST_ID      = 0x12345678;

	public:
		explicit TestCan(QObject * parent = nullptr);

	private slots:
		void testEmptyCanFrame();
		void testInvalidStandardCanFrame();
		void testInvalidExtendedCanFrame();
		void testValidService();
		void testTryValidService();
		void testInvalidService();
		void testReceivedResult();
		void testReceivedBroadcast();
		void testReceivedCommand();
		void testBroadcast();
		void testCommand();
		void testResult();
	};
}

#endif
