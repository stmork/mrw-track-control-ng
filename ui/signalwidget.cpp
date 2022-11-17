//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include "signalwidget.h"

using namespace mrw::ui;
using namespace mrw::ctrl;

SignalWidget::SignalWidget(
	QWidget      *     parent,
	SignalController * ctrl) :
	BaseWidget(parent),
	controller(ctrl)
{
}

void SignalWidget::setController(SignalController * ctrl)
{
	controller = ctrl;
}

static const QVector<QPointF> points
{
	QPointF(  -5.0,   50.0),
	QPointF(  15.0,   30.0),
	QPointF(  55.0,   70.0),
	QPointF(  35.0,   90.0)
};

void SignalWidget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)

	QPainter     painter(this);
	QPainterPath path;
	QFont        font = painter.font();

	Q_ASSERT(controller != nullptr);

	const int xSize = size().width();
	const int ySize = size().height();

	painter.setRenderHint(QPainter::Antialiasing, true);

	// TODO: Remove drawing the orientation later.
	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, xSize - 1, ySize - 1);

	// Unify coordinates
	painter.translate(xSize >> 1, ySize >> 1);
	painter.scale(xSize / 200.0, ySize / 200.0);

	// Draw switch name before rotating to prevent rotated font drawing.
	font.setPixelSize(50);
	painter.setFont(font);
	painter.setPen(Qt::yellow);
	painter.drawText(QRectF(
			controller->isDirection() ? -20 : -100,
			controller->isDirection() ? -80 : 30, 120, 50),
		Qt::AlignCenter | Qt::AlignHCenter, "N3");

	if (!controller->isDirection())
	{
		// Draw from left to right but rotate 180° if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	// Draw straight part of switch
	painter.setPen(QPen(Qt::red, 20.0));
	painter.drawLine( -100.0f, 0.0f, 100.0f, 0.0f);

	QPen pen(Qt::red, 8.0);

	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.drawLine(-30, 35, -30, 75);

	pen.setWidth(10.0);
	painter.setPen(pen);
	painter.drawLine(-30, 55, controller->hasDistant() || controller->hasShunting() ? 30 : 70, 55);

	if (controller->hasShunting())
	{
		painter.fillRect(10, 35, 40, 40, Qt::red);
	}
	else if (controller->hasDistant())
	{
		path.addPolygon(points);
		path.closeSubpath();
		painter.fillPath(path, QBrush(Qt::green));
	}

	if (controller->hasMain())
	{
		pen.setWidth(1);
		painter.setPen(pen);
		painter.setBrush(QBrush(Qt::red));
		painter.drawEllipse(50, 35, 40, 40);
	}
}
