//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTCANSERVICE_H
#define MRW_TEST_TESTCANSERVICE_H

#include <QObject>

#include <model/controller.h>
#include <model/device.h>

namespace mrw::test
{
	class TestCanService : public QObject
	{
		Q_OBJECT

		QString can_iface;
		QString can_plugin;

	public:
		explicit TestCanService(QObject * parent = nullptr);

	private slots:
		void testValidService();
		void testTryValidService();
		void testInvalidService();
		void testManualConnectService();
		void testReadWrite();
	};
}

#endif
