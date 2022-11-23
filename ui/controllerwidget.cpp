//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <util/clockservice.h>
#include <model/device.h>
#include <ui/controllerwidget.h>

using namespace mrw::util;
using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

ControllerWidget::ControllerWidget(
	QWidget *        parent,
	BaseController * ctrl) :
	BaseWidget(parent),
	base_controller(ctrl)
{
	list_item.setText("AaAa");
//	list_item.setData(Qt::UserRole, this);

	connect(&ClockService::instance(), &ClockService::Hz8, [this]()
	{
		counter++;
		if (isLockPending())
		{
			repaint();
		}
	});
}

void ControllerWidget::setController(BaseController * ctrl)
{
	Q_ASSERT(base_controller == nullptr);

	base_controller = ctrl;
}

void ControllerWidget::extend()
{
	setFixedWidth(height() * (1.0 + extensions() / Position::FRACTION));
}

void ControllerWidget::mousePressEvent(QMouseEvent * event)
{
	Q_UNUSED(event);

	emit clicked(&list_item);
}

bool ControllerWidget::lockVisible(const Device::LockState state) const
{
	return (state != Device::LockState::PENDING) || (counter & 1);
}

void ControllerWidget::drawLock(QPainter & painter, QColor color, const float x, const float y)
{
	painter.fillRect(x - LOCK_WIDTH * 0.5, y - LOCK_HEIGHT * 0.5, LOCK_WIDTH, LOCK_HEIGHT, color);
}

void ControllerWidget::drawSheared(
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

void ControllerWidget::prepareFailed(
	QPainter  & painter,
	const bool  fail)
{
	if (fail)
	{
		painter.setBackgroundMode(Qt::OpaqueMode);
		painter.setBackground(RED);
		painter.setPen(WHITE);
	}
	else
	{
		painter.setPen(YELLOW);
	}
}
