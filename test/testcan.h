//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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

		static constexpr mrw::can::ControllerId TEST_CTRL_ID = 0x1234 >> 2;
		static constexpr mrw::can::UnitNo       TEST_UNIT_NO = 0xa596;
		static constexpr mrw::can::UnitNo       TEST_LSB     =   0x96;
		static constexpr mrw::can::UnitNo       TEST_MSB     = 0xa5;
		static constexpr quint32                TEST_ID      = 0x1234a596;

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
		void testRequestPayload();
		void testResponsePayload();
		void testCopyRequest();
		void testCopyResponse();
	};
}

#endif
