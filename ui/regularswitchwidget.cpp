//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QPainter>

#include <ui/regularswitchwidget.h>
#include <ctrl/doublecrossswitchcontroller.h>

using namespace mrw::can;
using namespace mrw::ui;
using namespace mrw::ctrl;
using namespace mrw::model;

using LockState       = Device::LockState;
using ValueController = RegularSwitchController;

RegularSwitchWidget::RegularSwitchWidget(
	QWidget         *         parent,
	RegularSwitchController * ctrl) :
	ControllerWidget(parent, ctrl)
{
}

void RegularSwitchWidget::computeConnectors()
{
	RegularSwitchController * ctrl = controller<RegularSwitchController>();

	connector_list.clear();
	if (ctrl->isInclined())
	{
		const int ext = ctrl->isDirection() ? 0 : ctrl->extensions();

		if (ctrl->isRightBended())
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
		const int ext = ctrl->isDirection() ? ctrl->extensions() : 0;

		if (ctrl->isDirection() != ctrl->isRightBended())
		{
			connector_list.append(QPoint(2 + ext, 0));
		}
		else
		{
			connector_list.append(QPoint(2 + ext, 4));
		}
	}
}

void RegularSwitchWidget::paint(QPainter & painter)
{
	QFont        font    = painter.font();
	const bool   pending = lockVisible(base_controller->lock());
	const bool   is_inclined      =
		controller<RegularSwitchController>()->isInclined();
	const bool   is_right_bended  =
		controller<RegularSwitchController>()->isRightBended();
	const bool   flank_protection =
		controller<BaseSwitchController>()->hasFlankProtection();
	const bool   is_turn_out      =
		(controller<RegularSwitchController>()->isLeft() != is_right_bended) != is_inclined;

	Q_ASSERT(base_controller != nullptr);

	// Unify coordinates
	const float x_size = Position::FRACTION + extensions();
	const float y_size = Position::FRACTION * (1.0 + lines());
	const float x_pos  = base_controller->isDirection() != is_inclined ?
		x_size - Position::HALF :
		Position::HALF;
	const float y_pos  = Position::HALF;

	rescale(painter,
		x_size * SCALE / Position::HALF,
		y_size * SCALE / Position::HALF,
		x_pos * width() / x_size, y_pos * height() / y_size);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	prepareTextColor(painter, flank_protection);
	font.setPixelSize(FONT_SIZE);
	painter.setFont(font);
	painter.drawText(QRectF(
			base_controller->isDirection() == is_inclined ? -SCALE : -20,
			base_controller->isDirection() == is_right_bended ? -80 : 30, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, base_controller->name());

	if (is_right_bended != is_inclined)
	{
		// Draw always left handed but invert vertically if right handed.
		painter.scale( 1.0f, -1.0f);
	}
	if (!base_controller->isDirection())
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	const QColor section_color = sectionColor(base_controller->state());
	const QColor outside_color = sectionColor(SectionState::FREE);

	// Draw point lock
	drawLock(
		painter,
		base_controller->lock() == LockState::LOCKED ?
		section_color : WHITE,
		is_inclined ? -5 : -45, 0);

	QPen pen;
	pen.setCapStyle(Qt::FlatCap);
	pen.setWidth(RAIL_WIDTH);

	// Draw point part of switch
	pen.setColor(section_color);
	painter.setPen(pen);
	if (is_inclined)
	{
		drawSheared(painter, section_color, -50, 100, -85);
	}
	else
	{
		painter.drawLine(-SCALE - extensions() * SCALE * 0.5f, 0.0f, -70.0f, 0.0f);
	}

	// Draw curved part of switch
	drawSheared(
		painter,
		is_turn_out ? section_color : outside_color,
		is_inclined ? 50 : 0, -100, is_turn_out && pending ? 70 : 15);

	// Draw straight part of switch
	pen.setColor(!is_turn_out ? section_color : outside_color);
	painter.setPen(pen);
	painter.drawLine(
		!is_turn_out && pending ? (is_inclined ? 20 : -20.0f) : 80.0f, 0.0f,
		100.0f + extensions() * SCALE, 0.0f);

	// Draw connector markers
	drawConnectors(painter);
}

bool RegularSwitchWidget::isLockPending() const
{
	return base_controller->lock() == LockState::PENDING;
}
