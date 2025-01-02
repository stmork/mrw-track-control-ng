//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTRAILWIDGET_H
#define MRW_TEST_TESTRAILWIDGET_H

#include <QObject>

#include <mock/railcontrollermock.h>
#include <ui/railwidget.h>

#include "collections.h"

namespace mrw::test
{
	class RailWidgetMock : public mrw::ui::RailWidget
	{
		Q_OBJECT

	public:
		explicit RailWidgetMock(mrw::ctrl::RailControllerMock & mock) :
			RailWidget(nullptr, &mock)
		{
			setVerbose(true);
		}

		void test(RailWidget::Status & status)
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

	class TestRailWidget : public QObject, protected Collections
	{
		Q_OBJECT

	private:
		mrw::ctrl::RailControllerMock  mock;
		RailWidgetMock                 widget;
		mrw::ui::RailWidget::Status    status;

	public:
		explicit TestRailWidget(QObject * parent = nullptr);

	private slots:
		void init();

		void testSimple();
		void testPrepare();
		void testHavingLock();

		void testEnds();
	};
}

#endif
