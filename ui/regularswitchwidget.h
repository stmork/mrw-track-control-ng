//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_REGULARSWITCHWIDGET_H
#define MRW_UI_REGULARSWITCHWIDGET_H

#include <ui/controllerwidget.h>
#include <ctrl/regularswitchcontroller.h>

namespace mrw::ui
{
	class RegularSwitchWidget : public ControllerWidget
	{
		Q_OBJECT

	public:
		explicit RegularSwitchWidget(
			QWidget               *              parent     = nullptr,
			mrw::ctrl::RegularSwitchController * controller = nullptr);

		virtual void computeConnectors() override;

	protected:
		void paint(QPainter & painter) override;
		virtual bool isLockPending() const override;
	};
}

#endif
