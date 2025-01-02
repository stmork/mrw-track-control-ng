//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <util/clockservice.h>
#include <ui/opmodewidget.h>

using namespace mrw::util;
using namespace mrw::ui;

OpModeWidget::OpModeWidget(QWidget * parent) : BaseWidget(parent)
{
	connect(&ClockService::instance(), &ClockService::Hz2, [this] ()
	{
		if (blinking)
		{
			update();
			counter++;
		}
	});
	pen.setWidth(3.0);
}

void OpModeWidget::setLabel(
	const QString & text,
	const QColor  & color,
	const bool      blink)
{
	pen.setColor(color);
	label    = text;
	blinking = blink;
	counter  = 0;
	update();
}

void OpModeWidget::paint(QPainter & painter)
{
	QFont font;

	const int xSize = size().width();
	const int ySize = size().height();

	painter.setPen(pen);
	painter.drawRect(0, 0, xSize - 1, ySize - 1);

	if ((counter & 1) == 0)
	{
		// Draw switch name before rotating to prevent rotated font drawing.
		font.setPixelSize(ySize / 2);
		painter.setFont(font);
		painter.drawText(QRect(0, 0, xSize, ySize),
			Qt::AlignCenter | Qt::AlignHCenter, label);
	}
}
