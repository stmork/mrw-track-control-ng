//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
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
	Q_ASSERT(base_controller != nullptr);

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

void DoubleCrossSwitchWidget::prepare(DoubleCrossSwitchWidget::Status & status) const
{
	Q_ASSERT(base_controller != nullptr);

	controller<DoubleCrossSwitchController>()->status(status);

	status.section_color = sectionColor(status.section_state);
	status.outside_color = sectionColor(SectionState::FREE);
	status.pending       = lockVisible(status.lock_state);
}

void DoubleCrossSwitchWidget::paint(QPainter & painter)
{
	QPen                             pen;
	QFont                            font = painter.font();
	DoubleCrossSwitchWidget::Status  status;

	prepare(status);

	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	const QRectF  rect(-20, status.right_bended ? -85 : 35, 120, FONT_HEIGHT);

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
		status.lock_state == LockState::LOCKED ? status.section_color : WHITE,
		0, 0);

	pen.setCapStyle(Qt::FlatCap);
	pen.setWidth(RAIL_WIDTH);

	// Draw A segment
	drawSheared(painter,
		status.is_a ? status.section_color : status.outside_color, -50, -100,
		status.is_a && status.pending ?  70.0f :  15.0f, -RAIL_SLOPE);

	// Draw D segment
	drawSheared(painter,
		status.is_d ? status.section_color : status.outside_color,  50,  100,
		status.is_d && status.pending  ? -70.0f : -15.0f, -RAIL_SLOPE);

	// Draw B segment
	pen.setColor(status.is_b ? status.section_color : status.outside_color);
	painter.setPen(pen);
	painter.drawLine(-100.0f, 0.0f, status.is_b && status.pending  ? -27.0f : -80.0f, 0.0f);

	// Draw C segment
	pen.setColor(status.is_c ? status.section_color : status.outside_color);
	painter.setPen(pen);
	painter.drawLine( 100.0f, 0.0f, status.is_c && status.pending  ?  27.0f :  80.0f, 0.0f);

	// Draw connector markers
	drawConnectors(painter);
}

bool DoubleCrossSwitchWidget::hasLock() const
{
	return true;
}
