//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include "sectionwidget.h"

using namespace mrw::ui;

SectionWidget::SectionWidget(QWidget * parent) : BaseWidget(parent)
{
}

void SectionWidget::paint(QPainter & painter)
{
	rescale(painter, 200, 200);
}
