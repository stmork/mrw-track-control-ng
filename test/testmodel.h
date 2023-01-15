//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTMODEL_H
#define MRW_TEST_TESTMODEL_H

#include "testmodelbase.h"

#include <model/signal.h>

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
		void testLightSignalConfig(
			const unsigned                       pins,
			const mrw::model::Signal::SignalType type,
			const mrw::can::Command              command);
		void testFormSignalConfig(
			const unsigned                       inductors,
			const mrw::model::Signal::SignalType type,
			const mrw::can::Command              command);

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
		void testLightSignalConfig();
		void testFormSignalConfig();
		void testSectionConfig();
		void testSwitchConfig();
		void testSimpleLight();
		void testProfileLight();
	};
}

#endif
