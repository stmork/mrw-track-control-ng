//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_DOUBLECROSSSWITCHWIDGET_H
#define MRW_UI_DOUBLECROSSSWITCHWIDGET_H

#include <ui/basewidget.h>
#include <model/doublecrossswitch.h>

namespace mrw::ui
{
	class DoubleCrossSwitchWidget : public BaseWidget
	{
		Q_OBJECT

	public:
		explicit DoubleCrossSwitchWidget(QWidget * parent);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
