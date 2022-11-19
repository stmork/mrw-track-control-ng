//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "sectionwidget.h"

using namespace mrw::ui;

SectionWidget::SectionWidget(
		QWidget *                 parent,
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
	rescale(painter, 200, 200);

	// Draw point part of switch
	painter.setPen(QPen(sectionColor(controller->state()), 20.0));
	painter.drawLine(-100.0f, 0.0f, 100.0f, 0.0f);
}
