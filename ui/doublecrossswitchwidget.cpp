//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ui/doublecrossswitchwidget.h>

using namespace mrw::ui;
using namespace mrw::ctrl;
using namespace mrw::model;

DoubleCrossSwitchWidget::DoubleCrossSwitchWidget(
	QWidget           *           parent,
	DoubleCrossSwitchController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void DoubleCrossSwitchWidget::computeConnectors()
{
	connector_list.clear();
	if (base_controller->isDirection())
	{
		connector_list.append(QPoint(1, 0));
		connector_list.append(QPoint(3, 4));
	}
	else
	{
		connector_list.append(QPoint(3, 4));
		connector_list.append(QPoint(1, 0));
	}
}

void DoubleCrossSwitchWidget::paint(QPainter & painter)
{
	QFont  font          = painter.font();
	QColor section_color = sectionColor(base_controller->state());
	QColor outside_color = sectionColor(SectionState::FREE);
	const bool   pending = lockVisible(base_controller->lock());
	QPen   pen;

	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	prepareFailed(painter, base_controller->lock() == Device::LockState::FAIL);
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.drawText(QRectF(
			false ? -SCALE : -20,
			base_controller->isDirection() ? 35 : -85, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, base_controller->name());

	if (base_controller->isDirection())
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(-1.0f, 1.0f);
	}

	// Draw point lock
	drawLock(
		painter,
		base_controller->lock() == Device::LockState::LOCKED ?
		section_color : WHITE,
		0, 0);

	// Draw A segment
	drawSheared(painter,
		isA() ? section_color : outside_color, -50, -100,
		isA() && pending ?  70.0f :  15.0f, -RAIL_SLOPE);

	// Draw D segment
	drawSheared(painter,
		isD() ? section_color : outside_color,  50,  100,
		isD() && pending  ? -70.0f : -15.0f, -RAIL_SLOPE);

	pen.setCapStyle(Qt::FlatCap);
	pen.setWidth(RAIL_WIDTH);

	// Draw B segment
	pen.setColor(isB() ? section_color : outside_color);
	painter.setPen(pen);
	painter.drawLine(-100.0f, 0.0f, isB() && pending  ? -27.0f : -80.0f, 0.0f);

	// Draw C segment
	pen.setColor(isC() ? section_color : outside_color);
	painter.setPen(pen);
	painter.drawLine( 100.0f, 0.0f, isC() && pending  ?  27.0f :  80.0f, 0.0f);

	// Draw connector markers
	drawConnectors(painter);
}

bool DoubleCrossSwitchWidget::isLockPending() const
{
	return base_controller->lock() == Device::LockState::PENDING;
}

bool DoubleCrossSwitchWidget::isA() const
{
	const unsigned mask  = base_controller->isDirection() ? DoubleCrossSwitch::B_MASK : 0;

	return (switchState() & DoubleCrossSwitch::B_MASK) == mask;
}

bool DoubleCrossSwitchWidget::isB() const
{
	const unsigned mask  = base_controller->isDirection() ? DoubleCrossSwitch::B_MASK : 0;

	return (switchState() & DoubleCrossSwitch::B_MASK) != mask;
}

bool DoubleCrossSwitchWidget::isC() const
{
	const unsigned mask  = base_controller->isDirection() ? DoubleCrossSwitch::D_MASK : 0;

	return (switchState() & DoubleCrossSwitch::D_MASK) == mask;
}

bool DoubleCrossSwitchWidget::isD() const
{
	const unsigned mask  = base_controller->isDirection() ? DoubleCrossSwitch::D_MASK : 0;

	return (switchState() & DoubleCrossSwitch::D_MASK) != mask;
}

unsigned mrw::ui::DoubleCrossSwitchWidget::switchState() const
{
	return (unsigned)controller<DoubleCrossSwitchController>()->switchState();
}
