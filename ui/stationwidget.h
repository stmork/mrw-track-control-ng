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

	private:
		static const QPen pen;
		QString station_label;

	public:
		explicit StationWidget(QWidget * parent = nullptr);

		void setStationLabel(const QString & label);

	protected:
		void paintEvent(QPaintEvent * event) override;
	};
}

#endif // MRW_UI_STATIONWIDGET_H
