//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_SECTIONWIDGET_H
#define MRW_UI_SECTIONWIDGET_H

#include <ui/basewidget.h>
#include <model/rail.h>

namespace mrw::ui
{
	class SectionWidget : public mrw::ui::BaseWidget
	{
		Q_OBJECT
	public:
		explicit SectionWidget(QWidget * parent);

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
