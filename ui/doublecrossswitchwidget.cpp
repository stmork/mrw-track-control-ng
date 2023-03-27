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
	if (controller<BaseSwitchController>()->isRightBended())
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

	QPen                                 pen;
	QFont                                font = painter.font();
	DoubleCrossSwitchController::Status  status;

	controller<DoubleCrossSwitchController>()->status(status);

	const QColor section_color = sectionColor(status.section_state);
	const QColor outside_color = sectionColor(SectionState::FREE);
	const bool   pending       = lockVisible(status.lock_state);

	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	const QRectF  rect(
		false ? -SCALE : -20,
		status.right_bended ? -85 : 35, 120, FONT_HEIGHT);

	prepareTextColor(painter, status.has_flank_protection);
	font.setPixelSize(FONT_SIZE);
	painter.setFont(font);
	painter.drawText(rect, Qt::AlignCenter | Qt::AlignHCenter, status.name);

	if (!status.direction)
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	if (!status.right_bended)
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(1.0f, -1.0f);
	}

	// Draw point lock
	drawLock(
		painter,
		status.lock_state == LockState::LOCKED ?
		section_color : WHITE,
		0, 0);

	pen.setCapStyle(Qt::FlatCap);
	pen.setWidth(RAIL_WIDTH);

	// Draw A segment
	drawSheared(painter,
		isA(status) ? section_color : outside_color, -50, -100,
		isA(status) && pending ?  70.0f :  15.0f, -RAIL_SLOPE);

	// Draw D segment
	drawSheared(painter,
		isD(status) ? section_color : outside_color,  50,  100,
		isD(status) && pending  ? -70.0f : -15.0f, -RAIL_SLOPE);

	// Draw B segment
	pen.setColor(isB(status) ? section_color : outside_color);
	painter.setPen(pen);
	painter.drawLine(-100.0f, 0.0f, isB(status) && pending  ? -27.0f : -80.0f, 0.0f);

	// Draw C segment
	pen.setColor(isC(status) ? section_color : outside_color);
	painter.setPen(pen);
	painter.drawLine( 100.0f, 0.0f, isC(status) && pending  ?  27.0f :  80.0f, 0.0f);

	// Draw connector markers
	drawConnectors(painter);
}

bool DoubleCrossSwitchWidget::hasLock() const
{
	return true;
}

bool DoubleCrossSwitchWidget::isA(const DoubleCrossSwitchController::Status & status) const
{
	const unsigned mask = status.right_bended ? DoubleCrossSwitch::B_MASK : 0;

	return status.b_masked != mask;
}

bool DoubleCrossSwitchWidget::isB(const DoubleCrossSwitchController::Status & status) const
{
	const unsigned mask = status.right_bended ? DoubleCrossSwitch::B_MASK : 0;

	return status.b_masked == mask;
}

bool DoubleCrossSwitchWidget::isC(const DoubleCrossSwitchController::Status & status) const
{
	const unsigned mask = status.right_bended ? DoubleCrossSwitch::D_MASK : 0;

	return status.d_masked != mask;
}

bool DoubleCrossSwitchWidget::isD(const DoubleCrossSwitchController::Status & status) const
{
	const unsigned mask = status.right_bended ? DoubleCrossSwitch::D_MASK : 0;

	return status.d_masked == mask;
}
