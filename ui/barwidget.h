//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_BARWIDGET_H
#define MRW_UI_BARWIDGET_H

#include <ui/basewidget.h>

namespace mrw::ui
{
	/**
	 * The BarWidget displays all important colors needed to show all
	 * necessary signal and switch states. If there are missing colors one
	 * or more color channels of the hardware are broken and so the display
	 * is not operational safe.
	 *
	 * @image html BarWidget.jpg width=160
	 */
	class BarWidget : public BaseWidget
	{
		Q_OBJECT

	public:
		explicit BarWidget(QWidget * parent = nullptr);

	protected:
		void paint(QPainter & painter) override;

	private:
		static const QStringList colors;

	};
}

#endif
