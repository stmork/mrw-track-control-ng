//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ui/sectionwidget.h>
#include <model/position.h>

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using Curve = Position::Curve;

SectionWidget::SectionWidget(
	QWidget      *      parent,
	SectionController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void SectionWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QFont        font = painter.font();
	QPen         pen;
	Curve        curve  = base_controller->curve();

	const float  border = SCALE * (1.0 + extensions() / Position::FRACTION);
	const bool   a_ends = controller<SectionController>()->aEnds();
	const bool   b_ends = controller<SectionController>()->bEnds();

	rescale(painter,
		(Position::FRACTION + extensions()) * SCALE / Position::HALF,
		SCALE * 2.0);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	prepareFailed(painter, base_controller->lock() == Device::LockState::FAIL);
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.drawText(QRectF(
			base_controller->isDirection() ? border - 140 : 20 - border,
			base_controller->isDirection() ? 30 : -80, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, base_controller->name());

	if (!base_controller->isDirection())
	{
//		painter.scale(-1.0f, -1.0f);
	}

	// Draw rail
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(sectionColor(base_controller->state()));
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);
	painter.drawLine(curve == Curve::STRAIGHT ? -border : SCALE - border, 0.0f, border, 0.0f);

	// Rail bending to neighbour.
	if (curve != Curve::STRAIGHT)
	{
		const float height = SCALE + RAIL_WIDTH / 2;
		const float x      = SCALE / Position::HALF - border;

		if (curve == Curve::LEFT)
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
		if (base_controller->isDirection() != b_ends)
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
}
