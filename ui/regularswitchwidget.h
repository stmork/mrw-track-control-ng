//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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

	private:
		mrw::ctrl::RegularSwitchController * controller = nullptr;

	public:
		explicit RegularSwitchWidget(
			QWidget               *              parent     = nullptr,
			mrw::ctrl::RegularSwitchController * controller = nullptr);

		void setController(mrw::ctrl::RegularSwitchController * ctrl);

	protected:
		void paint(QPainter & painter) override;
		virtual bool isLockPending() const override;

	private:
		bool isTurnOut() const;
	};
}

#endif
