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
	QWidget         *         parent,
	ctrl::SectionController * ctrl) :
	ControllerWidget(parent),
	controller(ctrl)
{
}

void SectionWidget::setController(SectionController * ctrl)
{
	Q_ASSERT(controller == nullptr);

	controller = ctrl;
}

void SectionWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QFont        font = painter.font();
	QPen         pen;
	const float  border = SCALE * (1.0 + controller->extensions() / Position::FRACTION);

	rescale(painter, (Position::FRACTION + controller->extensions()) * SCALE / Position::HALF);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	pen.setColor(YELLOW);
	pen.setCapStyle(Qt::FlatCap);
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.setPen(pen);
	painter.drawText(QRectF(-60, -80, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, controller->name());

	// Draw point part of switch
	pen.setColor(sectionColor(controller->state()));
	pen.setWidth(RAIL_WIDTH);
	painter.setPen(pen);
	painter.drawLine(-border, 0.0f, border, 0.0f);

	if (controller->forwardEnds() || controller->backwardEnds())
	{
		if (controller->isDirection() != controller->backwardEnds())
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

void SectionWidget::extend()
{
	setFixedWidth(height() * (1.0 + controller->extensions() / Position::FRACTION));
}
