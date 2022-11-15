//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include "signalwidget.h"

using namespace mrw::ui;

SignalWidget::SignalWidget(QWidget * parent) : QWidget(parent)
{
}

void SignalWidget::setShunting(const bool shunt)
{
	shunting = shunt;
}

void SignalWidget::setDistant(const bool distant)
{
	distant_signal = distant;
}

void SignalWidget::setMain(const bool main)
{
	main_signal = main;
}

void SignalWidget::setDirection(const bool dir)
{
	direction = dir;
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

	const int xSize = size().width();
	const int ySize = size().height();

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
			isDirection() ? -20 : -100,
			isDirection() ? -90 : 30, 120, 50),
		Qt::AlignCenter | Qt::AlignHCenter, "N3");

	if (!isDirection())
	{
		// Draw from left to right but rotate 180° if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	// Draw straight part of switch
	painter.setPen(QPen(Qt::red, 20.0));
	painter.drawLine( -100.0f, 0.0f, 100.0f, 0.0f);

	QPen pen(Qt::red, 5.0);

	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.drawLine(-30, 35, -30, 75);

	pen.setWidth(10.0);
	painter.setPen(pen);
	painter.drawLine(-30, 55, hasDistant() || hasShunting() ? 30 : 70, 55);

	if (hasShunting())
	{
		painter.fillRect(10, 35, 40, 40, Qt::red);
	}
	else if (hasDistant())
	{
		path.addPolygon(points);
		path.closeSubpath();
		painter.fillPath(path, QBrush(Qt::green));
	}

	if (hasMain())
	{
		pen.setWidth(1);
		painter.setPen(pen);
		painter.setBrush(QBrush(Qt::red));
		painter.drawEllipse(50, 35, 40, 40);
	}
}

bool SignalWidget::isDirection() const
{
	return direction;
}

bool SignalWidget::hasShunting() const
{
	return shunting;
}

bool SignalWidget::hasDistant() const
{
	return distant_signal || (!hasMain());
}

bool SignalWidget::hasMain() const
{
	return main_signal;
}
