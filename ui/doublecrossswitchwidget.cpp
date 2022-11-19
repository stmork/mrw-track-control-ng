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
	rescale(painter);

	// TODO: Draw correctly!
	painter.setPen(QPen(sectionColor(controller->state()), 20.0));
	painter.drawLine(-SCALE,   0.0f, SCALE, 0.0f);
	painter.drawLine(-50.0f, -SCALE, 50.0f, SCALE);
}
