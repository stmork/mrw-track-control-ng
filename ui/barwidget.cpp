//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include "barwidget.h"

using namespace mrw::ui;

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
	font.setPixelSize(ySize / 3);
	painter.setPen(Qt::white);
	painter.setFont(font);

	// TODO: Make static
	const QStringList colors { "GN", "WS", "RT", "GE", "BL" };

	for (unsigned c = 0; c < 5; c++)
	{
		painter.drawText(QRect( c * xSize / 5, 2, xSize / 5, ySize / 3),
			Qt::AlignCenter | Qt::AlignHCenter, colors[c]);
	}

	// Unify coordinates
	painter.scale(xSize / 50.0, ySize / 10.0);

	painter.fillRect( 0, 5, 10, 5, Qt::green);
	painter.fillRect(10, 5, 10, 5, Qt::white);
	painter.fillRect(20, 5, 10, 5, Qt::red);
	painter.fillRect(30, 5, 10, 5, Qt::yellow);
	painter.fillRect(40, 5, 10, 5, Qt::blue);
}
