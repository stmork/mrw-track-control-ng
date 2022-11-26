//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ui/sectionwidget.h>
#include <model/position.h>

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Bending = Position::Bending;

SectionWidget::SectionWidget(
	QWidget     *    parent,
	RailController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void SectionWidget::computeConnectors()
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

void SectionWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QPen         pen;
	QFont        font    = painter.font();
	Bending      bending = base_controller->bending();

	const float  border  = SCALE * (1.0 + extensions() / Position::FRACTION);
	const bool   a_ends  = controller<RailController>()->aEnds();
	const bool   b_ends  = controller<RailController>()->bEnds();
	const bool   do_bend = (bending != Bending::STRAIGHT) && (!a_ends);

	rescale(painter,
		(Position::FRACTION + extensions()) * SCALE / Position::HALF,
		SCALE * 2.0);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	prepareFailed(painter, base_controller->lock() == Device::LockState::FAIL);
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.drawText(QRectF(
			base_controller->isDirection() ? border - 140 : 20 - border,
			base_controller->isDirection() == (bending != Bending::LEFT) ? 30 : -80, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, base_controller->name());

	if (!base_controller->isDirection())
	{
		painter.scale(-1.0f, -1.0f);
	}

	// Draw rail
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(sectionColor(base_controller->state()));
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);
	painter.drawLine(
		do_bend ? SCALE - border :  - border, 0.0f,
		border, 0.0f);

	// Rail bending to neighbour.
	if (do_bend)
	{
		const float height = SCALE + RAIL_WIDTH / 2;
		const float x      = SCALE / Position::HALF - border;

		if (bending == Bending::LEFT)
		{
			drawSheared(painter, pen.color(), x,  SCALE, -height,  RAIL_SLOPE);
		}
		else
		{
			drawSheared(painter, pen.color(), x, -SCALE,  height, -RAIL_SLOPE);
		}
	}

	// End rail drawing
	if (a_ends || b_ends)
	{
		if (a_ends)
		{
			// Draw from left to right but invert horizontally if counter direction.
			painter.scale(-1.0f, 1.0f);
		}

		path.moveTo(border - 15, -RAIL_WIDTH * 2);
		path.lineTo(border -  5, -RAIL_WIDTH * 2);
		path.lineTo(border -  5,  RAIL_WIDTH * 2);
		path.lineTo(border - 15,  RAIL_WIDTH * 2);
		pen.setWidth(RAIL_WIDTH * 0.5);
		painter.setPen(pen);
		painter.drawPath(path);
	}

	// Draw connector markers
	drawConnectors(painter);
}
