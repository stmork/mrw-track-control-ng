//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_RAILWIDGET_H
#define MRW_UI_RAILWIDGET_H

#include <ui/controllerwidget.h>
#include <ctrl/railcontroller.h>

namespace mrw::ui
{
	class RailWidget : public ControllerWidget
	{
		Q_OBJECT

	public:
		explicit RailWidget(
			QWidget          *          parent     = nullptr,
			mrw::ctrl::RailController * controller = nullptr);

		virtual void computeConnectors() override;

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
