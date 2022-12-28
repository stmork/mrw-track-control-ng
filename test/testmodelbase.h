//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTMODELBASE_H
#define MRW_TEST_TESTMODELBASE_H

#include <vector>

#include <QObject>

#include <model/modelrailway.h>
#include <model/section.h>
#include <model/railpart.h>

namespace mrw::test
{
	class TestModelBase : public QObject
	{
		Q_OBJECT

	protected:
		QString                             filename;
		mrw::model::ModelRailway      *     model = nullptr;
		std::vector<mrw::model::RailPart *> parts;

	public:
		explicit TestModelBase(const char * modelname, QObject * parent = nullptr);

	private slots:
		void initTestCase();
		void cleanupTestCase();

		void testInfo();
	};
}

#endif
