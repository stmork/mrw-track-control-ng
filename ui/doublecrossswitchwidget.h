//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_DOUBLECROSSSWITCHWIDGET_H
#define MRW_UI_DOUBLECROSSSWITCHWIDGET_H

#include <ui/controllerwidget.h>
#include <model/doublecrossswitch.h>
#include <ctrl/doublecrossswitchcontroller.h>

namespace mrw::ui
{
	class DoubleCrossSwitchWidget : public ControllerWidget
	{
		Q_OBJECT

	private:
		mrw::ctrl::DoubleCrossSwitchController * controller = nullptr;

	public:
		explicit DoubleCrossSwitchWidget(
			QWidget                 *                parent     = nullptr,
			mrw::ctrl::DoubleCrossSwitchController * controller = nullptr);

		void setController(mrw::ctrl::DoubleCrossSwitchController * ctrl);

	protected:
		void paint(QPainter & painter) override;
		virtual bool isLockPending() const override;

	private:
		bool isA() const;
		bool isB() const;
		bool isC() const;
		bool isD() const;
	};
}

#endif
