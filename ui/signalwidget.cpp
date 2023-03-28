//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QPainter>
#include <QPainterPath>

#include <ui/signalwidget.h>
#include <model/position.h>

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Bending   = Position::Bending;
using LockState = Device::LockState;
using Symbol    = Signal::Symbol;

static const QVector<QPointF> points
{
	QPointF(  -5.0,   50.0),
	QPointF(  15.0,   30.0),
	QPointF(  55.0,   70.0),
	QPointF(  35.0,   90.0)
};

SignalWidget::SignalWidget(
	QWidget      *     parent,
	SignalController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void SignalWidget::computeConnectors()
{
	BaseController::Status status;

	base_controller->status(status);
	connector_list.clear();
	if ((status.bending != Bending::STRAIGHT) && (status.extensions >= Position::FRACTION))
	{
		if (status.direction)
		{
			connector_list.append(QPoint(
					Position::FRACTION + status.extensions - Position::QUARTER,
					status.bending == Bending::RIGHT ? 4 : 0));
		}
		else
		{
			connector_list.append(QPoint(
					Position::QUARTER,
					status.bending == Bending::LEFT ? 4 : 0));
		}
	}
}

void SignalWidget::prepare(SignalWidget::Status & status) const
{
	Q_ASSERT(base_controller != nullptr);

	controller<SignalController>()->status(status);

	status.draw_lock     = (status.lock_state == LockState::PENDING) || (status.lock_state == LockState::LOCKED);
	status.section_color = sectionColor(status.section_state);

	// Predefine signal colors.
	status.main_color    = status.main_state    == Symbol::GO ? GREEN : RED;
	status.distant_color = status.distant_state == Symbol::GO ? GREEN : YELLOW;
	status.shunt_color   = status.shunt_state   == Symbol::GO ? WHITE : RED;

	if (status.has_main)
	{
		status.mast_color = status.main_color;
		if (status.main_state == Signal::Symbol::GO)
		{
			status.draw_distant = status.has_distant;
			if (status.draw_distant)
			{
				status.mast_color = status.distant_color;
			}
		}
		else
		{
			status.draw_shunt = status.has_shunting;
		}
	}
	else
	{
		if (!status.has_shunting || (status.section_state != SHUNTING) || (status.shunt_state != Symbol::GO))
		{
			status.draw_distant = status.has_distant;
		}

		if (status.draw_distant)
		{
			status.mast_color = status.distant_color;
		}
		else
		{
			status.draw_shunt = status.has_shunting;
		}
	}
}

void SignalWidget::paint(QPainter & painter)
{
	QPainterPath          path;
	QPen                  pen;
	QFont                 font = painter.font();
	SignalWidget::Status  status;

	prepare(status);

	const float   shift   = SCALE * status.extensions / Position::FRACTION;
	const float   border  = SCALE + shift;
	const float   start   = SCALE - border;
	const Bending do_bend =
		status.extensions >= Position::FRACTION ? status.bending : Bending::STRAIGHT;

	// Unify coordinates
	rescale(painter, (Position::FRACTION + status.extensions) * SCALE / Position::HALF);

	// Draw switch name before rotating to prevent rotated font drawing.
	prepareTextColor(painter);
	font.setPixelSize(FONT_SIZE);

	const QFontMetrics metrics(font);
	const float        text_width = metrics.horizontalAdvance(status.name) + 10;
	const QRectF       rect(
		status.direction ? -border : border - text_width,
		status.direction ? -80 : 30, text_width, FONT_HEIGHT);

	painter.setFont(font);
	painter.drawText(rect, Qt::AlignCenter | Qt::AlignHCenter, status.name);

	if (!status.direction)
	{
		// Draw from left to right but rotate 180° if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	// Draw straight part of rail
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(status.section_color);
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);

	if (status.draw_lock)
	{
		painter.drawLine( -border,    0.0f, start - 25.0f, 0.0f);
		painter.drawLine( start + 25.0f, 0.0f, do_bend == Bending::STRAIGHT ? border : border - SCALE, 0.0f);

		// Draw point lock
		if (lockVisible(status.lock_state))
		{
			drawLock(
				painter,
				status.lock_state == LockState::LOCKED ? status.section_color : WHITE,
				start, 0);
		}
	}
	else
	{
		painter.drawLine( -border, 0.0f, do_bend == Bending::STRAIGHT ? border : border - SCALE, 0.0f);
	}

	// Rail bending to neighbour.
	if (do_bend != Bending::STRAIGHT)
	{
		const float height = SCALE + RAIL_WIDTH / 2;
		const float x      = border - SCALE / Position::HALF;

		if (do_bend == Bending::RIGHT)
		{
			drawSheared(painter, pen.color(), x,  SCALE, -height, -RAIL_SLOPE);
		}
		else
		{
			drawSheared(painter, pen.color(), x, -SCALE,  height,  RAIL_SLOPE);
		}
	}

	// Draw mast foot
	pen.setColor(status.mast_color);
	pen.setWidth(8);
	pen.setCapStyle(Qt::FlatCap);
	painter.setPen(pen);
	painter.drawLine(start - 30, 35, start - 30, 75);

	// Draw mast
	pen.setWidth(10.0);
	painter.setPen(pen);
	painter.drawLine(start - 30, 55, start + (status.draw_distant || status.draw_shunt ? 30 : 70), 55);

	if (status.draw_shunt)
	{
		painter.fillRect(start + 10, 35, 40, 40, status.shunt_color);
	}

	if (status.draw_distant)
	{
		path.addPolygon(points);
		path.closeSubpath();
		path.translate(start, 0);
		painter.fillPath(path, QBrush(status.distant_color));
	}

	if (status.has_main)
	{
		pen.setColor(status.main_color);
		pen.setWidth(1);
		painter.setPen(pen);
		painter.setBrush(QBrush(status.main_color));
		painter.drawEllipse(start + 50, 35, 40, 40);
	}

	// Draw connector markers
	drawConnectors(painter);
}

bool SignalWidget::hasLock() const
{
	return true;
}
