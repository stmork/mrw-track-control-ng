//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTSIGNALWIDGET_H
#define MRW_TEST_TESTSIGNALWIDGET_H

#include <QObject>

#include <mock/signalcontrollermock.h>
#include <ui/signalwidget.h>

#include "collections.h"

namespace mrw::test
{
	class SignalWidgetMock : public mrw::ui::SignalWidget
	{
		Q_OBJECT

	public:
		explicit SignalWidgetMock(mrw::ctrl::SignalControllerMock & mock) :
			SignalWidget(nullptr, &mock)
		{
			setVerbose(true);
		}

		void test(SignalWidget::Status & status)
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

	class TestSignalWidget : public QObject, protected Collections
	{
		Q_OBJECT

	private:
		mrw::ctrl::SignalControllerMock  mock;
		SignalWidgetMock                 widget;
		mrw::ui::SignalWidget::Status    status;

	public:
		explicit TestSignalWidget(QObject * parent = nullptr);

	private slots:
		void init();

		void testSimple();
		void testPrepare();
		void testHavingLock();

		void testSh0();
		void testSh1();
		void testVr0();
		void testVr1();
		void testHp0();
		void testHp1();
		void testHp0WithDistant();
		void testHp1Vr0();
		void testHp1Vr1();
		void testHp00();
		void testHp0Sh1();
		void testConnections();
		void testBending();
		void testDrawLock();
	};
}

#endif
