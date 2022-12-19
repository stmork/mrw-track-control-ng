//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_OPMODEWIDGET_H
#define MRW_UI_OPMODEWIDGET_H

#include <ui/basewidget.h>

namespace mrw::ui
{
	class OpModeWidget : public BaseWidget
	{
		Q_OBJECT

		unsigned          counter = 0;
		bool              blinking = false;
		QPen              pen;
		QString           label;

	public:
		explicit OpModeWidget(QWidget * parent = nullptr);

		void setLabel(
			const QString & text,
			const QColor  & color = WHITE,
			const bool      blinking = false);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
