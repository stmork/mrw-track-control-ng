//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_SECTIONWIDGET_H
#define MRW_UI_SECTIONWIDGET_H

#include <ui/basewidget.h>
#include <ctrl/sectioncontroller.h>

namespace mrw::ui
{
	class SectionWidget : public BaseWidget
	{
		Q_OBJECT

	private:
		mrw::ctrl::SectionController * controller = nullptr;

	public:
		explicit SectionWidget(
			QWidget            *           parent     = nullptr,
			mrw::ctrl::SectionController * controller = nullptr);

		void setController(mrw::ctrl::SectionController * ctrl);

	protected:
		void paint(QPainter & painter) override;

	public slots:
		void extend();
	};
}

#endif
