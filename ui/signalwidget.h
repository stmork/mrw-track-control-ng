//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_SIGNALWIDGET_H
#define MRW_UI_SIGNALWIDGET_H

#include <ui/controllerwidget.h>
#include <ctrl/signalcontroller.h>

namespace mrw::ui
{
	class SignalWidget : public ControllerWidget
	{
		Q_OBJECT

	public:
		explicit SignalWidget(
			QWidget           *           parent     = nullptr,
			mrw::ctrl::SignalController * controller = nullptr);

		virtual void computeConnectors() override;

	protected:
		void paint(QPainter & painter) override;
	};
}

#endif
