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
	ControllerWidget(parent),
	controller(ctrl)
{
}

void DoubleCrossSwitchWidget::setController(DoubleCrossSwitchController * ctrl)
{
	Q_ASSERT(controller == nullptr);

	controller = ctrl;
}

void DoubleCrossSwitchWidget::paint(QPainter & painter)
{
	QFont  font          = painter.font();
	QColor section_color = sectionColor(controller->state());
	QColor outside_color = sectionColor(SectionState::FREE);
	const bool   pending = lockVisible(controller->lock());
	QPen   pen;

	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	prepareFailed(painter, controller->lock() == Device::LockState::FAIL);
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.drawText(QRectF(
			false ? -SCALE : -20,
			controller->isDirection() ? 35 : -85, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, controller->name());

	if (controller->isDirection())
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(-1.0f, 1.0f);
	}

	// Draw point lock
	drawLock(
		painter,
		controller->lock() == Device::LockState::LOCKED ?
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
}

bool DoubleCrossSwitchWidget::isLockPending() const
{
	return controller->lock() == Device::LockState::PENDING;
}

bool DoubleCrossSwitchWidget::isA() const
{
	const unsigned state = (unsigned)controller->switchState();
	const unsigned mask  = controller->isDirection() ? DoubleCrossSwitch::B_MASK : 0;

	return (state & DoubleCrossSwitch::B_MASK) == mask;
}

bool DoubleCrossSwitchWidget::isB() const
{
	const unsigned state = (unsigned)controller->switchState();
	const unsigned mask  = controller->isDirection() ? DoubleCrossSwitch::B_MASK : 0;

	return (state & DoubleCrossSwitch::B_MASK) != mask;
}

bool DoubleCrossSwitchWidget::isC() const
{
	const unsigned state = (unsigned)controller->switchState();
	const unsigned mask  = controller->isDirection() ? DoubleCrossSwitch::D_MASK : 0;

	return (state & DoubleCrossSwitch::D_MASK) == mask;
}

bool DoubleCrossSwitchWidget::isD() const
{
	const unsigned state = (unsigned)controller->switchState();
	const unsigned mask  = controller->isDirection() ? DoubleCrossSwitch::D_MASK : 0;

	return (state & DoubleCrossSwitch::D_MASK) != mask;
}
