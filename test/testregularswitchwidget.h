//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTREGULARSWITCHWIDGET_H
#define MRW_TEST_TESTREGULARSWITCHWIDGET_H

#include <QObject>

#include <mock/regularswitchcontrollermock.h>
#include <ui/regularswitchwidget.h>

#include "collections.h"

namespace mrw::test
{
	class RegularSwitchWidgetMock : public mrw::ui::RegularSwitchWidget
	{
		Q_OBJECT

	public:
		explicit RegularSwitchWidgetMock(mrw::ctrl::RegularSwitchControllerMock & mock) :
			RegularSwitchWidget(nullptr, &mock)
		{
			setVerbose(true);
		}

		void test(RegularSwitchWidget::Status & status)
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

	class TestRegularSwitchWidget : public QObject, protected Collections
	{
		Q_OBJECT

	private:
		mrw::ctrl::RegularSwitchControllerMock  mock;
		RegularSwitchWidgetMock                 widget;
		mrw::ui::RegularSwitchWidget::Status    status;

	public:
		explicit TestRegularSwitchWidget(QObject * parent = nullptr);

	private slots:
		void init();

		void testSimple();
		void testPrepare();
		void testHavingLock();

		void testLeft();
		void testRight();
		void testBending();
	};
}

#endif
