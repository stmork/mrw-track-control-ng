//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>
#include <QTime>

#include "clockwidget.h"

using namespace mrw::ui;

ClockWidget::ClockWidget(QWidget * parent) : BaseWidget(parent)
{
	timer.setInterval(1000);
	timer.setSingleShot(false);
	timer.start();

	connect(&timer, &QTimer::timeout, [this] ()
	{
		update();
	});
}

void ClockWidget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)

	QPainter     painter(this);
	QFont        font = painter.font();
	QTime        now  = QTime::currentTime();

	const int xSize = size().width();
	const int ySize = size().height();

	painter.setRenderHint(QPainter::Antialiasing, true);

	// TODO: Remove drawing the orientation later.
	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, xSize - 1, ySize - 1);

	// Draw switch name before rotating to prevent rotated font drawing.
	font.setPixelSize(ySize / 2);
	painter.setPen(Qt::white);
	painter.setFont(font);
	painter.drawText(QRect(0, 0, xSize, ySize),
		Qt::AlignCenter | Qt::AlignHCenter, now.toString("hh:mm:ss"));
}
