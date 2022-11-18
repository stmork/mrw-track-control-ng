//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "doublecrossswitchwidget.h"

using namespace mrw::ui;

DoubleCrossSwitchWidget::DoubleCrossSwitchWidget(QWidget * parent) :
	BaseWidget(parent)
{
}

void DoubleCrossSwitchWidget::paint(QPainter & painter)
{
	rescale(painter, 200, 200);
}
