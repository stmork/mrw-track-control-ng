//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_TEST_TESTRAILWIDGET_H
#define MRW_TEST_TESTRAILWIDGET_H

#include <QObject>

#include <mock/railcontrollermock.h>
#include <ui/railwidget.h>

namespace mrw::test
{
	class TestRailWidget : public QObject
	{
		Q_OBJECT

	private:
		mrw::ctrl::RailControllerMock  mock;
		mrw::ui::RailWidget            widget;
		mrw::ui::RailWidget::Status    status;

	public:
		explicit TestRailWidget(QObject * parent = nullptr);

	private slots:
		void testPrepare();
	};
}

#endif
