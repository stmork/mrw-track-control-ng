//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_REGULARSWITCHWIDGET_H
#define MRW_UI_REGULARSWITCHWIDGET_H

#include <can/commands.h>
#include <ui/basewidget.h>
#include <ctrl/regularswitchcontroller.h>

namespace mrw::ui
{
	class RegularSwitchWidget : public BaseWidget
	{
		Q_OBJECT

	private:
		bool left_handed = true;
		bool direction = true;

		mrw::ctrl::RegularSwitchController * controller = nullptr;
		mrw::can::SwitchState switch_state = mrw::can::SWITCH_STATE_LEFT;

	public:
		explicit RegularSwitchWidget(
			QWidget               *              parent = nullptr,
			mrw::ctrl::RegularSwitchController * controller = nullptr);

		void setLeft(const bool left = true);
		void setRight(const bool right = true);
		void setLeftHanded(const bool left = true);
		void setRightHanded(const bool right = true);
		void setDirection(const bool dir = true);

	protected:
		void paintEvent(QPaintEvent * event) override;

		bool isLeft() const;
		bool isRight() const;
		bool isRightHanded() const;
		bool isDirection() const;
	};
}

#endif
