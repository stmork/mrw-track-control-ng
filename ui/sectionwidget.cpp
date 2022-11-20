//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ui/sectionwidget.h>

using namespace mrw::ui;
using namespace mrw::ctrl;

SectionWidget::SectionWidget(
	QWidget         *         parent,
	ctrl::SectionController * ctrl) :
	BaseWidget(parent),
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

	rescale(painter);

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
	painter.drawLine(-SCALE, 0.0f, SCALE, 0.0f);

	if (controller->forwardEnds() || controller->backwardEnds())
	{
		if (controller->isDirection() != controller->backwardEnds())
		{
			// Draw from left to right but invert horizontally if counter direction.
			painter.scale(-1.0f, 1.0f);
		}

		path.moveTo( 90 - 5, -RAIL_WIDTH * 2);
		path.lineTo(100 - 5, -RAIL_WIDTH * 2);
		path.lineTo(100 - 5,  RAIL_WIDTH * 2);
		path.lineTo( 90 - 5,  RAIL_WIDTH * 2);
		pen.setWidth(RAIL_WIDTH * 0.5);
		painter.setPen(pen);
		painter.drawPath(path);
	}
}