//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTMODEL_H
#define MRW_TEST_TESTMODEL_H

#include "testmodelbase.h"

namespace mrw::test
{
	class TestModel : public TestModelBase
	{
		Q_OBJECT

	public:
		explicit TestModel(const char * modelname, QObject * parent = nullptr);
		virtual ~TestModel() = default;

	private:
		void testModule(mrw::model::Module * module);
		void testMuxConnection(mrw::model::MultiplexConnection * connection);
		void testSection(mrw::model::Section * section);
		void testAssemblyPart(mrw::model::AssemblyPart * part);

	private slots:
		void testModel();
		void testControllers();
		void testModules();
		void testRegions();
		void testSections();
		void testRegularSwitchStates();
		void tesDoubleCrossSwitchStates();
	};
}

#endif
