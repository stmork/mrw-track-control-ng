//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTSIGNALWIDGET_H
#define MRW_TEST_TESTSIGNALWIDGET_H

#include <QObject>

#include <mock/signalcontrollermock.h>
#include <ui/signalwidget.h>

namespace mrw::test
{
	class TestSignalWidget : public QObject
	{
		Q_OBJECT

	private:
		mrw::ctrl::SignalControllerMock  mock;
		mrw::ui::SignalWidget            widget;
		mrw::ui::SignalWidget::Status    status;

	public:
		explicit TestSignalWidget(QObject * parent = nullptr);

	private slots:
		void testPrepare();
	};
}

#endif
