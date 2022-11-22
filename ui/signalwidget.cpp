//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include <ui/signalwidget.h>

using namespace mrw::ui;
using namespace mrw::ctrl;

SignalWidget::SignalWidget(
	QWidget      *     parent,
	SignalController * ctrl) :
	ControllerWidget(parent),
	controller(ctrl)
{
}

void SignalWidget::setController(SignalController * ctrl)
{
	Q_ASSERT(controller == nullptr);

	controller = ctrl;
}

static const QVector<QPointF> points
{
	QPointF(  -5.0,   50.0),
	QPointF(  15.0,   30.0),
	QPointF(  55.0,   70.0),
	QPointF(  35.0,   90.0)
};

void SignalWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QFont        font = painter.font();
	const float  shift  = SCALE * controller->extensions() * 0.5;
	const float  border = SCALE + shift;

	Q_ASSERT(controller != nullptr);

	// Unify coordinates
	rescale(painter, (2.0 + controller->extensions()) * SCALE);

	// Draw switch name before rotating to prevent rotated font drawing.
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.setPen(YELLOW);
	painter.drawText(QRectF(
			controller->isDirection() ? -20 : -100,
			controller->isDirection() ? -80 : 30, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, controller->name());

	if (!controller->isDirection())
	{
		// Draw from left to right but rotate 180° if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	// Draw straight part of rail
	painter.setPen(QPen(sectionColor(controller->state()), 20.0));
	painter.drawLine( -border, 0.0f, border, 0.0f);

	QColor mast_color(RED);
	QPen   pen;
	QColor main_color    = controller->main() ==
		SignalController::TourState::STOP ? RED : GREEN;
	QColor distant_color = controller->distant() ==
		SignalController::TourState::STOP ? YELLOW : GREEN;
	QColor shunt_color   = controller->shunt() ==
		SignalController::TourState::STOP ? RED : WHITE;
	bool draw_shunt = false;
	bool draw_distant = false;

	if (controller->hasMain())
	{
		mast_color = main_color;
		if (controller->main() == SignalController::TourState::GO)
		{
			draw_distant = controller->hasDistant();
			if (draw_distant)
			{
				mast_color = distant_color;
			}
		}
		else
		{
			draw_shunt = controller->hasShunting();
		}
	}
	else
	{
		draw_distant = controller->hasDistant();
		if (draw_distant)
		{
			mast_color = distant_color;
		}
		else
		{
			draw_shunt = controller->hasShunting();
		}
	}

	// Draw mast foot
	pen.setColor(mast_color);
	pen.setWidth(8);
	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.drawLine(-30, 35, -30, 75);

	// Draw mast
	pen.setWidth(10.0);
	painter.setPen(pen);
	painter.drawLine(-30, 55, draw_distant || draw_shunt ? 30 : 70, 55);

	if (draw_shunt)
	{
		painter.fillRect(10, 35, 40, 40, shunt_color);
	}
	if (draw_distant)
	{
		path.addPolygon(points);
		path.closeSubpath();
		painter.fillPath(path, QBrush(distant_color));
	}

	if (controller->hasMain())
	{
		pen.setColor(main_color);
		pen.setWidth(1);
		painter.setPen(pen);
		painter.setBrush(QBrush(main_color));
		painter.drawEllipse(50, 35, 40, 40);
	}
}

void SignalWidget::extend()
{
	setFixedWidth(height() * (1.0 + controller->extensions() * 0.5));
}
