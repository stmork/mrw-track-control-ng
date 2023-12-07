//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTDOUBLECROSSWIDGETWIDGET_H
#define MRW_TEST_TESTDOUBLECROSSWIDGETWIDGET_H

#include <QObject>

#include <mock/doublecrossswitchcontrollermock.h>
#include <ui/doublecrossswitchwidget.h>

#include "collections.h"

namespace mrw::test
{
	class DoubleCrossSwitchWidgetMock : public mrw::ui::DoubleCrossSwitchWidget
	{
		Q_OBJECT

	public:
		explicit DoubleCrossSwitchWidgetMock(mrw::ctrl::DoubleCrossSwitchControllerMock & mock) :
			DoubleCrossSwitchWidget(nullptr, &mock)
		{
			setVerbose(true);
		}

		void test(DoubleCrossSwitchWidget::Status & status)
		{
			prepare(status);
			paintEvent(nullptr);
		}

		const QList<QPoint> & connectors()
		{
			computeConnectors();
			return connector_list;
		}
	};

	class TestDoubleCrossSwitchWidget : public QObject, protected Collections
	{
		Q_OBJECT

	private:
		mrw::ctrl::DoubleCrossSwitchControllerMock  mock;
		DoubleCrossSwitchWidgetMock                 widget;
		mrw::ui::DoubleCrossSwitchWidget::Status    status;

	public:
		explicit TestDoubleCrossSwitchWidget(QObject * parent = nullptr);

	private slots:
		void init();

		void testSimple();
		void testPrepare();
		void testHavingLock();

		void testSwitchStateAC();
		void testSwitchStateAD();
		void testSwitchStateBC();
		void testSwitchStateBD();
		void testLeftHanded();
		void testRightHanded();
	};
}

#endif
