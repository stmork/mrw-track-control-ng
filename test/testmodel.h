//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTMODEL_H
#define MRW_TEST_TESTMODEL_H

#include "testmodelbase.h"

#include <model/rail.h>
#include <model/abstractswitch.h>
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

		static const mrw::model::Rail * convert(
			const std::set<mrw::model::RailInfo>::iterator & it);
		static bool isSimpleLight(const mrw::model::Light * light);
		static bool hasCutOff(const mrw::model::AbstractSwitch * part);

	private slots:
		void init();

		void testModel();
		void testControllers();
		void testModules();
		void testRegions();
		void testSections();
		void testRegularSwitchStates();
		void testRegularSwitchFail();
		void testDoubleCrossSwitchStates();
		void testDoubleCrossSwitchFail();
		void testMainRail();
		void testSignals();
		void testDefaultPosition();
		void testParsingPosition();
		void testExtension();
		void testPosition();
		void testDevice();
		void testLight();
		void testEnumerator();
		void testLightSignalConfig();
		void testFormSignalConfig();
		void testSectionConfig();
		void testSwitchConfig();
		void testSimpleLightConfig();
		void testProfileLightConfig();
		void testStatistics();
	};
}

#endif
