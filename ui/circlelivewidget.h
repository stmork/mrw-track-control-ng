//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CIRCLELIVEWIDGET_H
#define MRW_UI_CIRCLELIVEWIDGET_H

#include <QTimer>

#include <ui/basewidget.h>

namespace mrw::ui
{
	/**
	 * The CircleLiveWidget shows a rotating bar which represents the
	 * responsiveness of the application. If the bar does not rotate any more
	 * the event loop got stuck.
	 */
	class CircleLiveWidget : public BaseWidget
	{
		Q_OBJECT

	private:
		static const QPen pen;
		unsigned          counter = 0;

	public:
		explicit CircleLiveWidget(QWidget * parent = nullptr);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
