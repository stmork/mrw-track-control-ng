//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CLOCKWIDGET_H
#define MRW_UI_CLOCKWIDGET_H

#include <QTimer>

#include <ui/basewidget.h>

namespace mrw::ui
{
	class ClockWidget : public BaseWidget
	{
		Q_OBJECT

	private:
		QTimer timer;

	public:
		explicit ClockWidget(QWidget * parent = nullptr);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif // MRW_UI_CLOCKWIDGET_H
