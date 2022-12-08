//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include <ui/signalwidget.h>
#include <model/position.h>

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Bending = Position::Bending;

SignalWidget::SignalWidget(
	QWidget      *     parent,
	SignalController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void SignalWidget::computeConnectors()
{
	const Bending  bending = base_controller->bending();
	const unsigned ext     = base_controller->extensions();

	connector_list.clear();
	if ((bending != Bending::STRAIGHT) && (ext >= Position::FRACTION))
	{
		if (base_controller->isDirection())
		{
			connector_list.append(QPoint(
					Position::FRACTION + ext - Position::QUARTER,
					bending == Bending::RIGHT ? 4 : 0));
		}
		else
		{
			connector_list.append(QPoint(
					Position::QUARTER,
					bending == Bending::LEFT ? 4 : 0));
		}
	}
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
	SignalController * signal_controller = controller<SignalController>();
	QPainterPath       path;
	QPen               pen;
	QFont              font     = painter.font();
	Bending            bending  = extensions() >= Position::FRACTION ?
		base_controller->bending() : Bending::STRAIGHT;

	const float        shift  = SCALE * extensions() / Position::FRACTION;
	const float        border = SCALE + shift;
	const float        start  = SCALE - border;

	const Signal::Symbol main_state    = signal_controller->main();
	const Signal::Symbol distant_state = signal_controller->distant();
	const Signal::Symbol shunt_state   = signal_controller->shunt();

	const bool has_main     = signal_controller->hasMain();
	const bool has_distant  = signal_controller->hasDistant();
	const bool has_shunting = signal_controller->hasShunting();

	// Unify coordinates
	rescale(painter, (Position::FRACTION + extensions()) * SCALE / Position::HALF);

	// Draw switch name before rotating to prevent rotated font drawing.
	prepareFailed(painter, base_controller->lock() == Device::LockState::FAIL);
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.drawText(QRectF(
			base_controller->isDirection() ? -border : border - 130,
			base_controller->isDirection() ? -80 : 30, 130, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, base_controller->name());

	if (!base_controller->isDirection())
	{
		// Draw from left to right but rotate 180° if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	// Draw straight part of rail
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(sectionColor(base_controller->state()));
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);
	painter.drawLine( -border, 0.0f, bending == Bending::STRAIGHT ? border : border - SCALE, 0.0f);

	// Rail bending to neighbour.
	if (bending != Bending::STRAIGHT)
	{
		const float height = SCALE + RAIL_WIDTH / 2;
		const float x      = border - SCALE / Position::HALF;

		if (bending == Bending::RIGHT)
		{
			drawSheared(painter, pen.color(), x,  SCALE, -height, -RAIL_SLOPE);
		}
		else
		{
			drawSheared(painter, pen.color(), x, -SCALE,  height,  RAIL_SLOPE);
		}
	}

	QColor mast_color(RED);
	QColor main_color    = main_state ==
		Signal::Symbol::STOP ? RED : GREEN;
	QColor distant_color = distant_state ==
		Signal::Symbol::STOP ? YELLOW : GREEN;
	QColor shunt_color   = shunt_state ==
		Signal::Symbol::STOP ? RED : WHITE;
	bool draw_shunt   = false;
	bool draw_distant = false;

	if (has_main)
	{
		mast_color = main_color;
		if (main_state == Signal::Symbol::GO)
		{
			draw_distant = has_distant;
			if (draw_distant)
			{
				mast_color = distant_color;
			}
		}
		else
		{
			draw_shunt = has_shunting;
		}
	}
	else
	{
		draw_distant = has_distant;
		if (draw_distant)
		{
			mast_color = distant_color;
		}
		else
		{
			draw_shunt = has_shunting;
		}
	}

	// Draw mast foot
	pen.setColor(mast_color);
	pen.setWidth(8);
	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.drawLine(start - 30, 35, start - 30, 75);

	// Draw mast
	pen.setWidth(10.0);
	painter.setPen(pen);
	painter.drawLine(start - 30, 55, start + (draw_distant || draw_shunt ? 30 : 70), 55);

	if (draw_shunt)
	{
		painter.fillRect(start + 10, 35, 40, 40, shunt_color);
	}

	if (draw_distant)
	{
		path.addPolygon(points);
		path.closeSubpath();
		path.translate(start, 0);
		painter.fillPath(path, QBrush(distant_color));
	}

	if (has_main)
	{
		pen.setColor(main_color);
		pen.setWidth(1);
		painter.setPen(pen);
		painter.setBrush(QBrush(main_color));
		painter.drawEllipse(start + 50, 35, 40, 40);
	}

	// Draw connector markers
	drawConnectors(painter);
}
