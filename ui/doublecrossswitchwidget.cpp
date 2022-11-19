//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "doublecrossswitchwidget.h"

using namespace mrw::ui;

DoubleCrossSwitchWidget::DoubleCrossSwitchWidget(
		QWidget *                                parent,
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
	rescale(painter, 200, 200);

	// TODO: Draw correctly!
	painter.setPen(QPen(sectionColor(controller->state()), 20.0));
	painter.drawLine(-100.0f, 0.0f, 100.0f, 0.0f);
	painter.drawLine(-50.0f, -100.0f, 50.0f, 100.0f);
}
