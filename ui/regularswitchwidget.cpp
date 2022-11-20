//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include <ui/regularswitchwidget.h>
#include <ctrl/doublecrossswitchcontroller.h>

using namespace mrw::can;
using namespace mrw::ui;
using namespace mrw::ctrl;
using namespace mrw::model;

RegularSwitchWidget::RegularSwitchWidget(
	QWidget         *         parent,
	RegularSwitchController * ctrl) :
	BaseWidget(parent),
	controller(ctrl)
{
}

void RegularSwitchWidget::setController(RegularSwitchController * ctrl)
{
	Q_ASSERT(controller == nullptr);

	controller = ctrl;
}

void RegularSwitchWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QFont        font = painter.font();

	Q_ASSERT(controller != nullptr);

	// Unify coordinates
	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.setPen(YELLOW);
	painter.drawText(QRectF(
			controller->isDirection() ? -SCALE : -20,
			controller->isDirection() == controller->isRightHanded() ? -80 : 30, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, controller->name());

	if (controller->isRightHanded())
	{
		// Draw always left handed but invert vertically if right handed.
		painter.scale( 1.0f, -1.0f);
	}
	if (!controller->isDirection())
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(-1.0f, -1.0f);
	}

	QColor section_color = sectionColor(controller->state());
	QColor outside_color = sectionColor(SectionState::FREE);

	// Draw curved part of switch
	drawSheared(
		painter,
		isTurnOut() ? section_color : outside_color,
		0, -100, isTurnOut() ? 70 : 15);

	// Draw point lock
	if (drawLock(controller->lock()))
	{
		drawLock(
			painter,
			controller->lock() == BaseController::LockState::LOCKED ?
			section_color : WHITE,
			-45, 0);
	}

	QPen pen;
	pen.setCapStyle(Qt::FlatCap);
	pen.setWidth(RAIL_WIDTH);

	// Draw point part of switch
	pen.setColor(section_color);
	painter.setPen(pen);
	painter.drawLine(-100.0f, 0.0f, -70.0f, 0.0f);

	// Draw straight part of switch
	pen.setColor(!isTurnOut() ? section_color : outside_color);
	painter.setPen(pen);
	painter.drawLine(!isTurnOut() ? -20.0f : 80.0f, 0.0f, 100.0f, 0.0f);
}

bool RegularSwitchWidget::isLockTransit() const
{
	return controller->lock() == BaseController::LockState::TRANSIT;
}

bool RegularSwitchWidget::isTurnOut() const
{
	return controller->isLeft() != controller->isRightHanded();
}
