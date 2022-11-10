//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTMODELBASE_H
#define MRW_TEST_TESTMODELBASE_H

#include <QObject>

#include <model/modelrailway.h>
#include <model/section.h>
#include <model/assemblypart.h>

namespace mrw::test
{
	class TestModelBase : public QObject
	{
		Q_OBJECT

	protected:
		QString                    filename;
		mrw::model::ModelRailway * model = nullptr;

	public:
		explicit TestModelBase(const char * modelname, QObject * parent = nullptr);

	private slots:
		void initTestCase();
		void cleanupTestCase();
		void testInfo();
	};
}

#endif
