//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QPainter>

#include "signalwidget.h"

using namespace mrw::ui;

SignalWidget::SignalWidget(QWidget * parent) : QWidget(parent)
{
}

void SignalWidget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event)

	QPainter     painter(this);
}
