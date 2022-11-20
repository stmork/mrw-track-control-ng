//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include <ui/circlelivewidget.h>
#include <util/clockservice.h>

using namespace mrw::ui;
using namespace mrw::util;

const QPen CircleLiveWidget::pen(Qt::white, 10.0);

CircleLiveWidget::CircleLiveWidget(QWidget * parent) : BaseWidget(parent)
{
	connect(&ClockService::instance(), &ClockService::Hz2, [this] ()
	{
		repaint();
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