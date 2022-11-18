//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_BARWIDGET_H
#define MRW_UI_BARWIDGET_H

#include <ui/basewidget.h>

namespace mrw::ui
{
	class BarWidget : public BaseWidget
	{
		Q_OBJECT
	public:
		explicit BarWidget(QWidget * parent = nullptr);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
