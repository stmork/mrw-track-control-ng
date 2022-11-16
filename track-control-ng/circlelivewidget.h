//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_CIRCLELIVEWIDGET_H
#define MRW_UI_CIRCLELIVEWIDGET_H

#include <QObject>
#include <QWidget>

namespace mrw::ui
{
	class CircleLiveWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit CircleLiveWidget(QWidget * parent = nullptr);

	protected:
		void paintEvent(QPaintEvent * event) override;
	};
}

#endif
