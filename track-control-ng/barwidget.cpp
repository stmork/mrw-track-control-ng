//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include "barwidget.h"

using namespace mrw::ui;

BarWidget::BarWidget(QWidget * parent) : QWidget(parent)
{
}

void BarWidget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)

	QPainter     painter(this);
	QPainterPath path;
	QFont        font = painter.font();

	const int xSize = size().width();
	const int ySize = size().height();

	painter.setRenderHint(QPainter::Antialiasing, true);

	// TODO: Remove drawing the orientation later.
	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, xSize - 1, ySize - 1);

	// Unify coordinates
	painter.translate(xSize >> 1, ySize >> 1);
	painter.scale(xSize / 200.0, ySize / 200.0);
}
