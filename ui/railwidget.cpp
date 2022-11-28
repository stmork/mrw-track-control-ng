//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

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
	const Bending  bending = base_controller->bending();
	const unsigned ext     = base_controller->extensions();

	connector_list.clear();
	if ((bending != Bending::STRAIGHT) &&
		(!controller<RailController>()->aEnds()))
	{
		if (base_controller->isDirection())
		{
			connector_list.append(QPoint(
					Position::QUARTER,
					bending == Bending::LEFT ? 4 * (1 + lines()) : 0));
		}
		else
		{
			connector_list.append(QPoint(
					Position::FRACTION + ext - Position::QUARTER,
					bending == Bending::RIGHT ? 4 * (1 + lines()) : 0));
		}
	}
}

void RailWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QPen         pen;
	QFont        font    = painter.font();
	Bending      bending = base_controller->bending();

	const float  border     = -SCALE - extensions() * SCALE / Position::HALF;
	const bool   a_ends     = controller<RailController>()->aEnds();
	const bool   b_ends     = controller<RailController>()->bEnds();
	const bool   is_dir     = base_controller->isDirection();
	const float  text_width = extensions() <= 2 ? 120 : 160;
	const bool   do_bend    = (bending != Bending::STRAIGHT) && (!a_ends);

	// Unify coordinates
	const float x_size = Position::FRACTION + extensions();
	const float y_size = Position::FRACTION * (1.0 + lines());
	const float x_pos  = is_dir ?
		x_size - Position::HALF :
		Position::HALF;
	const float y_pos  = (bending != Bending::LEFT) == is_dir ?
		y_size - Position::HALF :
		Position::HALF;

	rescale(painter,
		x_size * SCALE / Position::HALF,
		y_size * SCALE / Position::HALF,
		x_pos * width() / x_size, y_pos * height() / y_size);

	// Draw rail name before mirroring to prevent mirrored font drawing.
	prepareFailed(painter, base_controller->lock() == Device::LockState::FAIL);
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.drawText(QRectF(
			(is_dir == (bending != Bending::STRAIGHT)) || a_ends ? SCALE - text_width : -SCALE,
			is_dir == (bending != Bending::LEFT) ? 30 : -80, text_width, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, base_controller->name());

	if (!is_dir)
	{
		painter.scale(-1.0f, -1.0f);
	}

	const float y_offset = SCALE * (1.0 + lines() * 2.0);
	const float x_offset = y_offset / RAIL_SLOPE + SCALE / Position::HALF;

	// Straight rail drawing
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(sectionColor(base_controller->state()));
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);
	painter.drawLine(
		SCALE, 0, do_bend ? border + x_offset : border, 0);

	// Rail bending to neighbour.
	if (do_bend)
	{
		const float height = y_offset + RAIL_WIDTH / 2;
		const float x      = border + SCALE / Position::HALF;

		if (bending == Bending::LEFT)
		{
			drawSheared(painter, pen.color(), x,  y_offset, -height,  RAIL_SLOPE);
		}
		else
		{
			drawSheared(painter, pen.color(), x, -y_offset,  height, -RAIL_SLOPE);
		}
	}

	// Bended end of rail drawing
	if (a_ends || b_ends)
	{
		const float x = b_ends ? SCALE : -border;

		if (a_ends)
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
