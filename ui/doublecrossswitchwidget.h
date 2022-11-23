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

	public:
		explicit DoubleCrossSwitchWidget(
			QWidget                 *                parent     = nullptr,
			mrw::ctrl::DoubleCrossSwitchController * controller = nullptr);

	protected:
		void paint(QPainter & painter) override;
		virtual bool isLockPending() const override;

	private:
		bool isA() const;
		bool isB() const;
		bool isC() const;
		bool isD() const;
		unsigned switchState() const;
	};
}

#endif
