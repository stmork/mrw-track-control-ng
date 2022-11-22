//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <util/clockservice.h>
#include "ctrl/basecontroller.h"
#include <ui/basewidget.h>

using namespace mrw::util;
using namespace mrw::model;
using namespace mrw::ctrl;
using namespace mrw::ui;

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

BaseWidget::BaseWidget(QWidget * parent) : QWidget(parent), verbose(true)
{
	connect(&ClockService::instance(), &ClockService::Hz8, [this]()
	{
		counter++;
		if (isLockTransit())
		{
			repaint();
		}
	});
}

void BaseWidget::paintEvent(QPaintEvent * event)
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

bool BaseWidget::drawLock(const Device::LockState state) const
{
	return (state != Device::LockState::PENDING) || (counter & 1);
}

void BaseWidget::rescale(
	QPainter & painter,
	const float xSize, const float ySize,
	const float xPos,  const float yPos)
{
	painter.translate(xPos, yPos);
	painter.scale(width() / xSize, height() / ySize);
	if (verbose)
	{
		painter.fillRect(-95, -95, 5, 5, Qt::lightGray);
	}
}

void BaseWidget::rescale(
	QPainter & painter,
	const float xSize, const float ySize, const bool center)
{
	rescale(painter,
		xSize, ySize,
		center ? width()  * 0.5f : 0.0f,
		center ? height() * 0.5f : 0.0f);
}

void BaseWidget::drawLock(QPainter & painter, QColor color, const float x, const float y)
{
	painter.fillRect(x - LOCK_WIDTH * 0.5, y - LOCK_HEIGHT * 0.5, LOCK_WIDTH, LOCK_HEIGHT, color);
}

void BaseWidget::drawSheared(
	QPainter  & painter,
	QColor      color,
	const float x, const float y,
	const float height,
	const float slope)
{
	QPainterPath path;
	static constexpr float HALF = RAIL_WIDTH * 0.5f;
	const float distant = height / slope;

	path.moveTo(x - HALF, y);
	path.lineTo(x + HALF, y);
	path.lineTo(x + HALF - distant, y + height);
	path.lineTo(x - HALF - distant, y + height);
	path.closeSubpath();
	painter.fillPath(path, QBrush(color));
}
