//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <QPainterPath>

#include <util/clockservice.h>
#include <model/device.h>
#include <ui/controllerwidget.h>

using namespace mrw::util;
using namespace mrw::model;
using namespace mrw::ui;
using namespace mrw::ctrl;

using LockState = Device::LockState;

ControllerWidget::ControllerWidget(
	QWidget     *    parent,
	BaseController * ctrl) :
	BaseWidget(parent)
{
	setController(ctrl);

	connect(&ClockService::instance(), &ClockService::Hz8, [this]()
	{
		counter++;
		if (hasLock() && (base_controller->lock() == LockState::PENDING))
		{
			repaint();
		}
	});
}

void ControllerWidget::setController(BaseController * ctrl)
{
	Q_ASSERT(base_controller == nullptr);

	base_controller = ctrl;

	if (base_controller != nullptr)
	{
		list_item.setText(base_controller->name());
		list_item.setData(USER_ROLE, QVariant::fromValue(base_controller));
		connect(base_controller, &BaseController::reposition, this, &ControllerWidget::reposition);

		computeConnectors();
	}
}

bool ControllerWidget::isConnector(const QPoint & point) const
{
	const QPoint base(
		x() * Position::FRACTION / gridSize(),
		y() * Position::FRACTION / gridSize());

	return std::any_of(
			connector_list.begin(),
			connector_list.end(),
			[&] (const QPoint & local)
	{
		const QPoint compare = local + base;

		return compare == point;
	});
}

void ControllerWidget::reposition()
{
	setFixedHeight(gridSize() * (1.0 + base_controller->lines()));
	extend();
	move(base_controller->position()->point() * gridSize() / Position::FRACTION);
}

void ControllerWidget::extend()
{
	const float rel_height = height() / (1.0 + base_controller->lines());

	setFixedWidth(rel_height * (1.0 + base_controller->extensions() / Position::FRACTION));

	computeConnectors();
}

void ControllerWidget::computeConnectors()
{
	connector_list.clear();
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
	const float x,
	const float y,
	const float height,
	const float slope)
{
	QPainterPath path;
	static constexpr float HALF    = RAIL_WIDTH * 0.5f;
	const            float distant = height / slope;

	path.moveTo(x - HALF, y);
	path.lineTo(x + HALF, y);
	path.lineTo(x + HALF - distant, y + height);
	path.lineTo(x - HALF - distant, y + height);
	path.closeSubpath();
	painter.fillPath(path, QBrush(color));
}

void ControllerWidget::prepareTextColor(
	QPainter  & painter,
	const bool  flank_protection)
{
	const LockState lock_state = base_controller->lock();

	if (lock_state == LockState::FAIL)
	{
		painter.setBackgroundMode(Qt::OpaqueMode);
		painter.setBackground(RED);
		painter.setPen(WHITE);
	}
	else
	{
		painter.setPen(flank_protection && (lock_state == LockState::LOCKED) ? GREEN : YELLOW);
	}
}

void ControllerWidget::drawConnectors(QPainter & painter)
{
	if (verbose)
	{
		painter.resetTransform();
		for (const QPoint & conn : connector_list)
		{
			const QPoint point(
				conn.x() * width()  / ( base_controller->extensions() + Position::FRACTION),
				conn.y() * height() / ((base_controller->lines() + 1.0) * Position::FRACTION));

			painter.fillRect(point.x() - 2, point.y() - 2, 5, 5, Qt::magenta);
		}
	}
}
