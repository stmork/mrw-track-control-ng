//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTCAN_H
#define MRW_TEST_TESTCAN_H

#include <QObject>

namespace mrw::test
{
	class TestCan : public QObject
	{
		Q_OBJECT

	public:
		explicit TestCan(QObject * parent = nullptr);

	private slots:
		void testEmptyCanFrame();
		void testCanFrame();
	};
}

#endif
