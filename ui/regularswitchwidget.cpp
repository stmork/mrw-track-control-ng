//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QPainter>

#include <ui/regularswitchwidget.h>

using namespace mrw::can;
using namespace mrw::ui;
using namespace mrw::ctrl;
using namespace mrw::model;

using LockState = Device::LockState;

RegularSwitchWidget::RegularSwitchWidget(
	QWidget         *         parent,
	RegularSwitchController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void RegularSwitchWidget::computeConnectors()
{
	RegularSwitchController::Status status;

	controller<RegularSwitchController>()->status(status);
	connector_list.clear();
	if (status.inclined)
	{
		const int ext = status.direction ? 0 : status.extensions;

		if (status.right_bended)
		{
			connector_list.append(QPoint(3 + ext, 0));
			connector_list.append(QPoint(1 + ext, 4));
		}
		else
		{
			connector_list.append(QPoint(1 + ext, 0));
			connector_list.append(QPoint(3 + ext, 4));
		}
	}
	else
	{
		const int ext = status.direction ? status.extensions : 0;

		if (status.direction != status.right_bended)
		{
			connector_list.append(QPoint(2 + ext, 0));
		}
		else
		{
			connector_list.append(QPoint(2 + ext, 4));
		}
	}
}

void RegularSwitchWidget::prepare(RegularSwitchWidget::Status & status) const
{
	Q_ASSERT(base_controller != nullptr);

	controller<RegularSwitchController>()->status(status);

	status.section_color = sectionColor(status.section_state);
	status.outside_color = sectionColor(SectionState::FREE);

	status.pending       = lockVisible(status.lock_state);
	status.is_turn_out   = (status.left != status.right_bended) != status.inclined;
}

void RegularSwitchWidget::paint(QPainter & painter)
{
	QFont                        font = painter.font();
	RegularSwitchWidget::Status  status;

	prepare(status);

	// Unify coordinates
	const float x_size = Position::FRACTION + status.extensions;
	const float y_size = Position::FRACTION * (1.0 + status.lines);
	const float x_pos  = status.direction != status.inclined ?
		x_size - Position::HALF :
		Position::HALF;
	const float y_pos  = Position::HALF;

	rescale(painter,
		x_size * SCALE / Position::HALF,
		y_size * SCALE / Position::HALF,
		x_pos * width() / x_size, y_pos * height() / y_size);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	const QRectF  rect(
		status.direction == status.inclined ? -SCALE : -20,
		status.direction == status.right_bended ? -80 : 30, 120, FONT_HEIGHT);

	prepareTextColor(painter, status.has_flank_protection);
	font.setPixelSize(FONT_SIZE);
	painter.setFont(font);
	painter.drawText(rect, Qt::AlignCenter | Qt::AlignHCenter, status.name);

	if (status.right_bended != status.inclined)
	{
		// Draw always left handed but invert vertically if right handed.
		painter.scale( 1.0f, -1.0f);
	}
	if (!status.direction)
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	// Draw point lock
	drawLock(
		painter,
		status.lock_state == LockState::LOCKED ? status.section_color : WHITE,
		status.inclined ? -5 : -45, 0);

	QPen pen;
	pen.setCapStyle(Qt::FlatCap);
	pen.setWidth(RAIL_WIDTH);

	// Draw point part of switch
	pen.setColor(status.section_color);
	painter.setPen(pen);
	if (status.inclined)
	{
		drawSheared(painter, status.section_color, -50, 100, -85);
	}
	else
	{
		painter.drawLine(-SCALE - status.extensions * SCALE * 0.5f, 0.0f, -70.0f, 0.0f);
	}

	// Draw curved part of switch
	drawSheared(
		painter,
		status.is_turn_out ? status.section_color : status.outside_color,
		status.inclined ? 50 : 0, -100, status.is_turn_out && status.pending ? 70 : 15);

	// Draw straight part of switch
	pen.setColor(!status.is_turn_out ? status.section_color : status.outside_color);
	painter.setPen(pen);
	painter.drawLine(
		!status.is_turn_out && status.pending ? (status.inclined ? 20 : -20.0f) : 80.0f, 0.0f,
		100.0f + status.extensions * SCALE, 0.0f);

	// Draw connector markers
	drawConnectors(painter);
}

bool RegularSwitchWidget::hasLock() const
{
	return true;
}
