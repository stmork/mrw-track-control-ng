//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTMODEL_H
#define MRW_TEST_TESTMODEL_H

#include <QObject>

#include <model/modelrailway.h>

namespace mrw::test
{
	class TestModel : public QObject
	{
		Q_OBJECT

	protected:
		QString                    filename;
		mrw::model::ModelRailway * model = nullptr;

	public:
		explicit TestModel(const char * modelname, QObject * parent = nullptr);

	private slots:
		void initTestCase();
		void cleanupTestCase();

		void testModel();
		void testControllers();
		void testAreas();
		void testSections();
	};
}

#endif
