//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_OPMODEWIDGET_H
#define MRW_UI_OPMODEWIDGET_H

#include <ui/basewidget.h>

namespace mrw::ui
{
	/**
	 * The OpModeWidget displays the operational mode consisting of a single
	 * upper case letter. The operation mode may have a color and the letter
	 * may blink.
	 *
	 * @image html OpModeWidget.jpg width=40
	 */
	class OpModeWidget : public BaseWidget
	{
		Q_OBJECT

		unsigned          counter = 0;
		bool              blinking = false;
		QPen              pen;
		QString           label;

	public:
		explicit OpModeWidget(QWidget * parent = nullptr);

		/**
		 * This method sets the internal state of this OpModeWidget.
		 *
		 * @param text The letter to display.
		 * @param color The draw color.
		 * @param blinking True if the text should blink.
		 */
		void setLabel(
			const QString & text,
			const QColor  & color = WHITE,
			const bool      blinking = false);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
