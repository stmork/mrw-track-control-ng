//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_SIGNALWIDGET_H
#define MRW_UI_SIGNALWIDGET_H

#include <QWidget>

namespace mrw::ui
{
	class SignalWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit SignalWidget(QWidget * parent = nullptr);

	protected:
		void paintEvent(QPaintEvent * event) override;
	};
}

#endif
