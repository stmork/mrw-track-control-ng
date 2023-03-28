//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QPainterPath>

#include <ui/railwidget.h>
#include <model/position.h>

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Bending = Position::Bending;

RailWidget::RailWidget(
	QWidget     *    parent,
	RailController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void RailWidget::computeConnectors()
{
	RailController::Status status;

	controller<RailController>()->status(status);
	connector_list.clear();
	if ((status.bending != Bending::STRAIGHT) && (!status.a_ends))
	{
		if (status.direction)
		{
			connector_list.append(QPoint(
					Position::QUARTER,
					status.bending == Bending::LEFT ? 4 * (1 + status.lines) : 0));
		}
		else
		{
			connector_list.append(QPoint(
					Position::FRACTION + status.extensions - Position::QUARTER,
					status.bending == Bending::RIGHT ? 4 * (1 + status.lines) : 0));
		}
	}
}

void RailWidget::prepare(RailWidget::Status & status) const
{
	Q_ASSERT(base_controller != nullptr);

	controller<RailController>()->status(status);

	status.do_bend = (status.bending != Bending::STRAIGHT) && (!status.a_ends);
	status.any_end = status.a_ends || status.b_ends;
}

void RailWidget::paint(QPainter & painter)
{
	QPainterPath        path;
	QPen                pen;
	QFont               font = painter.font();
	RailWidget::Status  status;

	prepare(status);

	const float border     = -SCALE - status.extensions * SCALE / Position::HALF;
	const float text_width = status.extensions <= 2 ? 120 : 160;

	// Unify coordinates
	const float x_size = Position::FRACTION + status.extensions;
	const float y_size = Position::FRACTION * (1.0 + status.lines);
	const float x_pos  = status.direction ?
		x_size - Position::HALF :
		Position::HALF;
	const float y_pos  = (status.bending != Bending::LEFT) == status.direction ?
		y_size - Position::HALF :
		Position::HALF;

	rescale(painter,
		x_size * SCALE / Position::HALF,
		y_size * SCALE / Position::HALF,
		x_pos * width() / x_size, y_pos * height() / y_size);

	// Draw rail name before mirroring to prevent mirrored font drawing.
	const QRectF  rect(
		(status.direction == (status.bending != Bending::STRAIGHT)) || status.a_ends ? SCALE - text_width : -SCALE,
		(status.direction == (status.bending != Bending::LEFT)) ? 30 : -80, text_width, FONT_HEIGHT);

	prepareTextColor(painter);
	font.setPixelSize(FONT_SIZE);
	painter.setFont(font);
	painter.drawText(rect, Qt::AlignCenter | Qt::AlignHCenter, status.name);

	if (!status.direction)
	{
		painter.scale(-1.0f, -1.0f);
	}

	const float y_offset = SCALE * (1.0 + status.lines * 2.0);
	const float x_offset = y_offset / RAIL_SLOPE + SCALE / Position::HALF;

	// Straight rail drawing
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(sectionColor(status.section_state));
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);
	painter.drawLine(SCALE, 0, status.do_bend ? border + x_offset : border, 0);

	// Rail bending to neighbour.
	if (status.do_bend)
	{
		const float height = y_offset + RAIL_WIDTH / 2;
		const float x      = border + SCALE / Position::HALF;

		if (status.bending == Bending::LEFT)
		{
			drawSheared(painter, pen.color(), x,  y_offset, -height,  RAIL_SLOPE);
		}
		else
		{
			drawSheared(painter, pen.color(), x, -y_offset,  height, -RAIL_SLOPE);
		}
	}

	// Bended end of rail drawing
	if (status.any_end)
	{
		const float x = status.b_ends ? SCALE : -border;

		if (status.a_ends)
		{
			// Draw from left to right but invert horizontally if counter direction.
			painter.scale(-1.0f, 1.0f);
		}

		path.moveTo(x - 15, -RAIL_WIDTH * 2);
		path.lineTo(x -  5, -RAIL_WIDTH * 2);
		path.lineTo(x -  5,  RAIL_WIDTH * 2);
		path.lineTo(x - 15,  RAIL_WIDTH * 2);
		pen.setWidth(RAIL_WIDTH * 0.5);
		painter.setPen(pen);
		painter.drawPath(path);
	}

	// Draw connector markers
	drawConnectors(painter);
}
