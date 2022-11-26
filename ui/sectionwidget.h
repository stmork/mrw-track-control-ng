//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_SECTIONWIDGET_H
#define MRW_UI_SECTIONWIDGET_H

#include <ui/controllerwidget.h>
#include <ctrl/railcontroller.h>

namespace mrw::ui
{
	class SectionWidget : public ControllerWidget
	{
		Q_OBJECT

	public:
		explicit SectionWidget(
			QWidget          *          parent     = nullptr,
			mrw::ctrl::RailController * controller = nullptr);

		virtual void computeConnectors() override;

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
