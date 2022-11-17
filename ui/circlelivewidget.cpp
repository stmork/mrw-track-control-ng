//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include "circlelivewidget.h"

using namespace mrw::ui;

const QPen CircleLiveWidget::pen(Qt::white, 10.0);

CircleLiveWidget::CircleLiveWidget(QWidget * parent) : BaseWidget(parent)
{
	timer.setInterval(1000);
	timer.setSingleShot(false);
	timer.start();

	connect(&timer, &QTimer::timeout, [this] ()
	{
		update();
		counter++;
	});
}

void CircleLiveWidget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)

	QPainter     painter(this);

	const int xSize = size().width();
	const int ySize = size().height();

	painter.setRenderHint(QPainter::Antialiasing, true);

	// TODO: Remove drawing the orientation later.
	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, xSize - 1, ySize - 1);

	// Unify coordinates
	painter.translate(xSize >> 1, ySize >> 1);
	painter.scale(xSize / 200.0, ySize / 200.0);
	painter.rotate(counter * 45.0);

	painter.setPen(pen);
	painter.drawLine(-100, 0, 100, 0);
}
