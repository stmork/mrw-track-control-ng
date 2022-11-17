//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_BASEWIDGET_H
#define MRW_UI_BASEWIDGET_H

#include <QWidget>

namespace mrw::ui
{
	class BaseWidget : public QWidget
	{
		Q_OBJECT

	public:
		static const int SIZE = 40;

		explicit BaseWidget(QWidget * parent = nullptr);
	};
}

#endif
