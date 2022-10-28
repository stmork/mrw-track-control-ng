//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTUTIL_H
#define MRW_TEST_TESTUTIL_H

#include <QObject>

namespace mrw::test
{
	class TestUtil : public QObject
	{
		Q_OBJECT

	public:
		explicit TestUtil(QObject * parent = nullptr);

	private slots:
		void testMethod();
	};
}

#endif // MRW_TEST_TESTUTIL_H
