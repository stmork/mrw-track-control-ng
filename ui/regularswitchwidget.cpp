//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include "regularswitchwidget.h"

using namespace mrw::can;
using namespace mrw::ui;
using namespace mrw::model;

RegularSwitchWidget::RegularSwitchWidget(
	QWidget               *              parent,
	mrw::ctrl::RegularSwitchController * ctrl) :
	BaseWidget(parent),
	controller(ctrl)
{
}

void mrw::ui::RegularSwitchWidget::setController(mrw::ctrl::RegularSwitchController * ctrl)
{
	Q_ASSERT(controller == nullptr);

	controller = ctrl;
}

static const QVector<QPointF> points_active
{
	QPointF( -52.5,  -15.0),
	QPointF( -10.0, -100.0),
	QPointF(  10.0, -100.0),
	QPointF( -32.5,  -15.0)
};

static const QVector<QPointF> points_inactive
{
	QPointF( -25.0,  -70.0),
	QPointF( -10.0, -100.0),
	QPointF(  10.0, -100.0),
	QPointF(  -5.0,  -70.0)
};

void RegularSwitchWidget::paint(QPainter & painter)
{
	QPainterPath path;
	QFont        font = painter.font();

	Q_ASSERT(controller != nullptr);

	// Unify coordinates
	rescale(painter, 200, 200);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	font.setPixelSize(50);
	painter.setFont(font);
	painter.setPen(YELLOW);
	painter.drawText(QRectF(
			controller->isDirection() ? -100 : -20,
			controller->isRightHanded() ? -80 : 30, 120, 50),
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
	path.addPolygon(isTurnOut() ? points_active : points_inactive);
	path.closeSubpath();
	painter.fillPath(path, QBrush(isTurnOut() ? section_color : outside_color));

	// Draw point lock
	painter.fillRect(-65.0, -11.0, 40.0, 22.0, WHITE);

	// Draw point part of switch
	painter.setPen(QPen(section_color, 20.0));
	painter.drawLine(-100.0f, 0.0f, -80.0f, 0.0f);

	// Draw straight part of switch
	painter.setPen(QPen(!isTurnOut() ? section_color : outside_color, 20.0));
	painter.drawLine(!isTurnOut() ? -10.0f : 70.0f, 0.0f, 100.0f, 0.0f);
}

bool mrw::ui::RegularSwitchWidget::isTurnOut() const
{
	return controller->isLeft() != controller->isRightHanded();
}
