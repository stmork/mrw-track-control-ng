//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTMODEL_H
#define MRW_TEST_TESTMODEL_H

#include <QObject>

#include <model/modelrailway.h>
#include <model/section.h>
#include <model/assemblypart.h>

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

	private:
		void testModule(mrw::model::Module * module);
		void testSection(mrw::model::Section * section);
		void testAssemblyPart(mrw::model::AssemblyPart * part);

	private slots:
		void initTestCase();
		void cleanupTestCase();

		void testModel();
		void testControllers();
		void testModules();
		void testAreas();
		void testSections();
	};
}

#endif
