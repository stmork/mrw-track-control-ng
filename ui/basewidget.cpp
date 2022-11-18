//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "basewidget.h"

using namespace mrw::ui;

BaseWidget::BaseWidget(QWidget * parent) : QWidget(parent)
{
}

void mrw::ui::BaseWidget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);

#if 1
	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, width() - 1, height() - 1);
#endif

	paint(painter);
}

void BaseWidget::rescale(
	QPainter & painter,
	const float xSize, const float ySize,
	const float xPos,  const float yPos)
{
	painter.translate(xPos, yPos);
	painter.scale(width() / xSize, height() / ySize);
}

void BaseWidget::rescale(
	QPainter & painter,
	const float xSize, const float ySize, const bool center)
{
	rescale(painter, xSize, ySize, center ? width() * 0.5f : 0.0f, center ? height() * 0.5f : 0.0f);
}
