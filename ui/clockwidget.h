//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CLOCKWIDGET_H
#define MRW_UI_CLOCKWIDGET_H

#include <QTimer>

#include <ui/basewidget.h>

namespace mrw::ui
{
	/**
	 * The ClockWidget displays the actual time.
	 *
	 * @image html ClockWidget.jpg width=160
	 */
	class ClockWidget : public BaseWidget
	{
		Q_OBJECT

	public:
		explicit ClockWidget(QWidget * parent = nullptr);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
