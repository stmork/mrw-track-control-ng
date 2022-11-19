//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "sectionwidget.h"

using namespace mrw::ui;

SectionWidget::SectionWidget(
	QWidget         *         parent,
	ctrl::SectionController * ctrl) :
	BaseWidget(parent),
	controller(ctrl)
{
}

void SectionWidget::setController(mrw::ctrl::SectionController * ctrl)
{
	Q_ASSERT(controller == nullptr);

	controller = ctrl;
}

void SectionWidget::paint(QPainter & painter)
{
	QFont        font = painter.font();

	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.setPen(YELLOW);
	painter.drawText(QRectF(-60, -80, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, controller->name());

	// Draw point part of switch
	painter.setPen(QPen(sectionColor(controller->state()), 20.0));
	painter.drawLine(-SCALE, 0.0f, SCALE, 0.0f);
}
