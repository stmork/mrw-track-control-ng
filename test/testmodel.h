//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
		void testSection(model::Region * region, mrw::model::Section * section);
		void testAssemblyPart(model::Section * section, mrw::model::AssemblyPart * part);

	private slots:
		void init();

		void testModel();
		void testControllers();
		void testModules();
		void testRegions();
		void testSections();
		void testRegularSwitchStates();
		void testDoubleCrossSwitchStates();
		void testDefaultPosition();
		void testParsingPosition();
		void testExtension();
		void testPosition();
		void testDevice();
		void testEnumerator();
	};
}

#endif
