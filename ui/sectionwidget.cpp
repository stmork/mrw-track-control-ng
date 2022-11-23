//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ui/sectionwidget.h>
#include <model/position.h>

using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

SectionWidget::SectionWidget(
	QWidget   *         parent,
	SectionController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void SectionWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QFont        font = painter.font();
	QPen         pen;
	const float  border = SCALE * (1.0 + extensions() / Position::FRACTION);
	const bool   forward_ends  = controller<SectionController>()->forwardEnds();
	const bool   backward_ends = controller<SectionController>()->backwardEnds();

	rescale(painter,
		(Position::FRACTION + extensions()) * SCALE / Position::HALF,
		SCALE * 2.0);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	prepareFailed(painter, base_controller->lock() == Device::LockState::FAIL);
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.drawText(QRectF(
			base_controller->isDirection() ? border - 120 : -border,
			base_controller->isDirection() ? 30 : -80, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, base_controller->name());

	// Draw point part of switch
	pen.setCapStyle(Qt::FlatCap);
	pen.setColor(sectionColor(base_controller->state()));
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);
	painter.drawLine(-border, 0.0f, border, 0.0f);

	if (forward_ends || backward_ends)
	{
		if (base_controller->isDirection() != backward_ends)
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
