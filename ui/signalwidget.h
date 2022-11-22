//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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

	private:
		mrw::ctrl::SignalController * controller = nullptr;

	public:
		explicit SignalWidget(
			QWidget           *           parent     = nullptr,
			mrw::ctrl::SignalController * controller = nullptr);

		void setController(mrw::ctrl::SignalController * ctrl);

	protected:
		void paint(QPainter & painter) override;

	public slots:
		void extend();
	};
}

#endif
