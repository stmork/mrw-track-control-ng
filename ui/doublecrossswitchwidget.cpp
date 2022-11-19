//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "doublecrossswitchwidget.h"

using namespace mrw::ui;

DoubleCrossSwitchWidget::DoubleCrossSwitchWidget(
	QWidget                 *                parent,
	mrw::ctrl::DoubleCrossSwitchController * ctrl) :
	BaseWidget(parent),
	controller(ctrl)
{
}

void DoubleCrossSwitchWidget::setController(mrw::ctrl::DoubleCrossSwitchController * ctrl)
{
	Q_ASSERT(controller == nullptr);

	controller = ctrl;
}

void DoubleCrossSwitchWidget::paint(QPainter & painter)
{
	QFont        font = painter.font();

	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.setPen(YELLOW);
	painter.drawText(QRectF(
			controller->isDirection() ? -SCALE : -20,
			controller->isDirection() ? 30 : -80, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, controller->name());

	// TODO: Draw correctly!
	painter.setPen(QPen(sectionColor(controller->state()), 20.0));
	painter.drawLine(-SCALE,   0.0f, SCALE, 0.0f);
	painter.drawLine(-50.0f, -SCALE, 50.0f, SCALE);
}
