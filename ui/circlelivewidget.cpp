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

void CircleLiveWidget::paint(QPainter & painter)
{
	// Unify coordinates
	rescale(painter, 200.0, 200.0);

	painter.rotate(counter * 45.0);
	painter.setPen(pen);
	painter.drawLine(-100, 0, 100, 0);
}
