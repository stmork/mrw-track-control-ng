//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <ui/doublecrossswitchwidget.h>

using namespace mrw::ui;
using namespace mrw::ctrl;
using namespace mrw::model;

using LockState = Device::LockState;

DoubleCrossSwitchWidget::DoubleCrossSwitchWidget(
	QWidget           *           parent,
	DoubleCrossSwitchController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void DoubleCrossSwitchWidget::computeConnectors()
{
	connector_list.clear();
	if (controller<DoubleCrossSwitchController>()->isRightBended())
	{
		connector_list.append(QPoint(1, 0));
		connector_list.append(QPoint(3, 4));
	}
	else
	{
		connector_list.append(QPoint(3, 0));
		connector_list.append(QPoint(1, 4));
	}
}

void DoubleCrossSwitchWidget::paint(QPainter & painter)
{
	Q_ASSERT(base_controller != nullptr);

	QPen         pen;
	QFont        font             = painter.font();
	const QColor section_color    = sectionColor(base_controller->state());
	const QColor outside_color    = sectionColor(SectionState::FREE);

	const LockState    lock_state = base_controller->lock();
	const bool         in_dir     = base_controller->isDirection();
	const bool         pending    = lockVisible(lock_state);
	const bool         flank_protection =
		controller<BaseSwitchController>()->hasFlankProtection();
	const bool   is_right_bended  =
		controller<DoubleCrossSwitchController>()->isRightBended();

	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	const QString name = base_controller->name();
	const QRectF  rect(
		false ? -SCALE : -20,
		is_right_bended ? -85 : 35, 120, FONT_HEIGHT);

	prepareTextColor(painter, flank_protection);
	font.setPixelSize(FONT_SIZE);
	painter.setFont(font);
	painter.drawText(rect, Qt::AlignCenter | Qt::AlignHCenter, name);

	if (!in_dir)
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	if (!is_right_bended)
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(1.0f, -1.0f);
	}

	// Draw point lock
	drawLock(
		painter,
		lock_state == LockState::LOCKED ?
		section_color : WHITE,
		0, 0);

	pen.setCapStyle(Qt::FlatCap);
	pen.setWidth(RAIL_WIDTH);

	// Draw A segment
	drawSheared(painter,
		isA() ? section_color : outside_color, -50, -100,
		isA() && pending ?  70.0f :  15.0f, -RAIL_SLOPE);

	// Draw D segment
	drawSheared(painter,
		isD() ? section_color : outside_color,  50,  100,
		isD() && pending  ? -70.0f : -15.0f, -RAIL_SLOPE);

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
	return base_controller->lock() == LockState::PENDING;
}

bool DoubleCrossSwitchWidget::isA() const
{
	const bool is_right_bended = controller<DoubleCrossSwitchController>()->isRightBended();
	const unsigned mask        = is_right_bended ? DoubleCrossSwitch::B_MASK : 0;

	return (switchState() & DoubleCrossSwitch::B_MASK) != mask;
}

bool DoubleCrossSwitchWidget::isB() const
{
	const bool is_right_bended = controller<DoubleCrossSwitchController>()->isRightBended();
	const unsigned mask        = is_right_bended ? DoubleCrossSwitch::B_MASK : 0;

	return (switchState() & DoubleCrossSwitch::B_MASK) == mask;
}

bool DoubleCrossSwitchWidget::isC() const
{
	const bool is_right_bended = controller<DoubleCrossSwitchController>()->isRightBended();
	const unsigned mask        = is_right_bended ? DoubleCrossSwitch::D_MASK : 0;

	return (switchState() & DoubleCrossSwitch::D_MASK) != mask;
}

bool DoubleCrossSwitchWidget::isD() const
{
	const bool is_right_bended = controller<DoubleCrossSwitchController>()->isRightBended();
	const unsigned mask        = is_right_bended ? DoubleCrossSwitch::D_MASK : 0;

	return (switchState() & DoubleCrossSwitch::D_MASK) == mask;
}

unsigned DoubleCrossSwitchWidget::switchState() const
{
	return (unsigned)controller<DoubleCrossSwitchController>()->switchState();
}
