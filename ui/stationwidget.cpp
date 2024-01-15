//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QPainter>

#include "stationwidget.h"

using namespace mrw::ui;

const QPen StationWidget::pen(Qt::white, 3.0);

StationWidget::StationWidget(QWidget * parent) : BaseWidget(parent)
{
}

void StationWidget::setStationLabel(const QString & label)
{
	station_label = label;
	update();
}

void StationWidget::paint(QPainter & painter)
{
	QFont font;

	const int xSize = size().width();
	const int ySize = size().height();

	painter.setPen(pen);
	painter.drawRect(0, 0, xSize - 1, ySize - 1);

	// Draw switch name before rotating to prevent rotated font drawing.
	font.setPixelSize(ySize / 2);
	painter.setFont(font);
	painter.drawText(QRect(0, 0, xSize, ySize),
		Qt::AlignCenter | Qt::AlignHCenter, station_label);
}
