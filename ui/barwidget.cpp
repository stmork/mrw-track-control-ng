//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QPainter>
#include <QPainterPath>

#include "barwidget.h"

using namespace mrw::ui;

const QStringList BarWidget::colors { "GN", "WS", "RT", "GE", "BL" };

BarWidget::BarWidget(QWidget * parent) : BaseWidget(parent)
{
}

void BarWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QFont        font = painter.font();

	const int xSize = size().width();
	const int ySize = size().height();

	// Define fonts for color labels.
	font.setBold(true);
	font.setPixelSize(ySize / 3);
	painter.setPen(Qt::white);
	painter.setFont(font);

	for (unsigned c = 0; c < 5; c++)
	{
		painter.drawText(QRect( c * xSize / 5, 2, xSize / 5, ySize / 3),
			Qt::AlignCenter | Qt::AlignHCenter, colors[c]);
	}

	// Unify coordinates
	painter.scale(xSize / 50.0, ySize / 10.0);

	painter.fillRect( 0, 5, 10, 5, GREEN);
	painter.fillRect(10, 5, 10, 5, WHITE);
	painter.fillRect(20, 5, 10, 5, RED);
	painter.fillRect(30, 5, 10, 5, YELLOW);
	painter.fillRect(40, 5, 10, 5, BLUE);
}
