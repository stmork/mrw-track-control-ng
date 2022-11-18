//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CIRCLELIVEWIDGET_H
#define MRW_UI_CIRCLELIVEWIDGET_H

#include <QTimer>

#include <ui/basewidget.h>

namespace mrw::ui
{
	class CircleLiveWidget : public BaseWidget
	{
		Q_OBJECT

	private:
		static const QPen pen;
		QTimer            timer;
		unsigned          counter = 0;

	public:
		explicit CircleLiveWidget(QWidget * parent = nullptr);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
