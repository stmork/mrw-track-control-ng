//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include "regularswitchwidget.h"

using namespace mrw::can;
using namespace mrw::ui;

RegularSwitchWidget::RegularSwitchWidget(QWidget * parent) : QWidget(parent)
{
}

void RegularSwitchWidget::setRight(const bool right)
{
	switch_state = right ? SWITCH_STATE_RIGHT : SWITCH_STATE_LEFT;
}

void RegularSwitchWidget::setLeft(const bool left)
{
	switch_state = left ? SWITCH_STATE_LEFT : SWITCH_STATE_RIGHT;
}

void RegularSwitchWidget::setDirection(const bool dir)
{
	direction = dir;
}

static const QVector<QPointF> points
{
	QPointF( -47.5,  -25.0),
	QPointF( -10.0, -100.0),
	QPointF(  10.0, -100.0),
	QPointF( -27.5,  -25.0)
};

void RegularSwitchWidget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)

	QPainter     painter(this);
	QPainterPath path;

	const int xSize = size().width();
	const int ySize = size().height();

	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, xSize - 1, ySize - 1);

	painter.translate(xSize >> 1, ySize >> 1);
	painter.scale(xSize / 200.0, ySize / 200.0);
	if (isRight())
	{
		painter.scale( 1.0f, -1.0f);
	}
	if (!isDirection())
	{
		painter.scale(-1.0f,  1.0f);
	}
//	painter.setBackgroundMode(Qt::TransparentMode);

	path.addPolygon(points);
	path.closeSubpath();
	painter.fillPath(path, QBrush(Qt::green));

	painter.fillRect(-65.0, -20.0, 40.0, 40.0, Qt::white);

	painter.setPen(QPen(Qt::red, 20.0));
	painter.drawLine(-100.0f, 0.0f, -80.0f, 0.0f);
	painter.drawLine( -10.0f, 0.0f, 100.0f, 0.0f);

	QFont font = painter.font();
	font.setPixelSize(30);
	painter.setFont(font);
	painter.setPen(QColor(192, 192, 0));
	painter.drawText(QRectF(-100, 30, 110, 30), Qt::AlignCenter | Qt::AlignBottom, "207");
}

bool RegularSwitchWidget::isRight() const
{
	return switch_state == SWITCH_STATE_RIGHT;
}

bool RegularSwitchWidget::isDirection() const
{
	return direction;
}
