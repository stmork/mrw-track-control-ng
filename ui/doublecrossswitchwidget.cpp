//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <ui/doublecrossswitchwidget.h>

using namespace mrw::ui;
using namespace mrw::model;

DoubleCrossSwitchWidget::DoubleCrossSwitchWidget(
	QWidget                 *                parent,
	mrw::ctrl::DoubleCrossSwitchController * ctrl) :
	BaseWidget(parent),
	controller(ctrl)
{
}

void DoubleCrossSwitchWidget::setController(mrw::ctrl::DoubleCrossSwitchController * ctrl)
{
	Q_ASSERT(controller == nullptr);

	controller = ctrl;
}

void DoubleCrossSwitchWidget::paint(QPainter & painter)
{
	QFont  font          = painter.font();
	QColor section_color = sectionColor(controller->state());
	QColor outside_color = sectionColor(SectionState::FREE);
	QPen   pen;

	rescale(painter);

	// Draw switch name before mirroring to prevent mirrored font drawing.
	font.setPixelSize(FONT_HEIGHT);
	painter.setFont(font);
	painter.setPen(YELLOW);
	painter.drawText(QRectF(
			false ? -SCALE : -20,
			!controller->isDirection() ? 30 : -80, 120, FONT_HEIGHT),
		Qt::AlignCenter | Qt::AlignHCenter, controller->name());

	if (!controller->isDirection())
	{
		// Draw from left to right but invert horizontally if counter direction.
		painter.scale(-1.0f, 1.0f);
	}

	// Draw point lock
	painter.fillRect(-20.0, -11.0, 40.0, 22.0, WHITE);

	drawSheared(painter,
		isA() ? section_color : outside_color, -50, -100,
		isA() ?  70.0f :  15.0f, -RAIL_SLOPE);
	drawSheared(painter,
		isD() ? section_color : outside_color,  50,  100,
		isD() ? -70.0f : -15.0f, -RAIL_SLOPE);

	pen.setCapStyle(Qt::FlatCap);
	pen.setWidth(RAIL_WIDTH);

	// Draw point part of switch
	pen.setColor(isB() ? section_color : outside_color);
	painter.setPen(pen);
	painter.drawLine(-100.0f, 0.0f, isB() ? -27.0f : -80.0f, 0.0f);

	// Draw point part of switch
	pen.setColor(isC() ? section_color : outside_color);
	painter.setPen(pen);
	painter.drawLine( 100.0f, 0.0f, isC() ?  27.0f :  80.0f, 0.0f);
}

bool mrw::ui::DoubleCrossSwitchWidget::isA() const
{
	const unsigned state = (unsigned)controller->switchState();

	return (state & DoubleCrossSwitch::B_MASK) == 0;
}

bool mrw::ui::DoubleCrossSwitchWidget::isB() const
{
	const unsigned state = (unsigned)controller->switchState();

	return (state & DoubleCrossSwitch::B_MASK) != 0;
}

bool mrw::ui::DoubleCrossSwitchWidget::isC() const
{
	const unsigned state = (unsigned)controller->switchState();

	return (state & DoubleCrossSwitch::D_MASK) == 0;
}

bool mrw::ui::DoubleCrossSwitchWidget::isD() const
{
	const unsigned state = (unsigned)controller->switchState();

	return (state & DoubleCrossSwitch::D_MASK) != 0;
}
