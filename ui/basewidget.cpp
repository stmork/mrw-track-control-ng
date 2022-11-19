//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "basewidget.h"

using namespace mrw::ui;
using namespace mrw::model;

const QColor BaseWidget::GREEN(Qt::green);
const QColor BaseWidget::WHITE(Qt::white);
const QColor BaseWidget::RED(Qt::red);
const QColor BaseWidget::YELLOW(Qt::yellow);
const QColor BaseWidget::BLUE(64, 64, 255);
const QColor BaseWidget::RED_LIGHT(255, 128, 128);

const std::unordered_map<SectionState, QColor> BaseWidget::color_map
{
	{ SectionState::FREE,     YELLOW },
	{ SectionState::SHUNTING, BLUE },
	{ SectionState::TOUR,     GREEN },
	{ SectionState::OCCUPIED, RED },
	{ SectionState::PASSED,   RED_LIGHT }
};

BaseWidget::BaseWidget(QWidget * parent) : QWidget(parent)
{
}

void mrw::ui::BaseWidget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);

	if (verbose)
	{
		painter.setPen(Qt::gray);
		painter.drawRect(0, 0, width() - 1, height() - 1);
	}

	paint(painter);
}

QColor BaseWidget::sectionColor(const SectionState state)
{
	auto it = color_map.find(state);

	return it != color_map.end() ? it->second : Qt::white;
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
