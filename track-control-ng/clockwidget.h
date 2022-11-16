//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CLOCKWIDGET_H
#define MRW_UI_CLOCKWIDGET_H

#include <QObject>
#include <QWidget>

namespace mrw::ui
{
	class ClockWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit ClockWidget(QWidget * parent = nullptr);

	protected:
		void paintEvent(QPaintEvent * event) override;
	};
}

#endif // MRW_UI_CLOCKWIDGET_H
