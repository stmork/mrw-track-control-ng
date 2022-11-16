//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_STATIONWIDGET_H
#define MRW_UI_STATIONWIDGET_H

#include <QObject>
#include <QWidget>

namespace mrw::ui
{
	class StationWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit StationWidget(QWidget * parent = nullptr);

	protected:
		void paintEvent(QPaintEvent * event) override;
	};
}

#endif // MRW_UI_STATIONWIDGET_H
