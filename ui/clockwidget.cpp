//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QPainter>
#include <QTime>

#include <ui/clockwidget.h>
#include <util/clockservice.h>

using namespace mrw::ui;
using namespace mrw::util;

ClockWidget::ClockWidget(QWidget * parent) : BaseWidget(parent)
{
	connect(
		&ClockService::instance(), &ClockService::Hz1,
		this, qOverload<>(&QWidget::update));
}

void ClockWidget::paint(QPainter & painter)
{
	QFont        font = painter.font();
	QTime        now  = QTime::currentTime();

	const int xSize = size().width();
	const int ySize = size().height();

	// Draw switch name before rotating to prevent rotated font drawing.
	font.setPixelSize(ySize / 2);
	painter.setPen(Qt::white);
	painter.setFont(font);
	painter.drawText(QRect(0, 0, xSize, ySize),
		Qt::AlignCenter | Qt::AlignHCenter, now.toString("hh:mm:ss"));
}
