//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UI_REGULARSWITCHWIDGET_H
#define MRW_UI_REGULARSWITCHWIDGET_H

#include <QWidget>

#include <can/commands.h>

namespace mrw::ui
{
	class RegularSwitchWidget : public QWidget
	{
		Q_OBJECT

	private:
		bool left_handed = true;
		bool direction = true;

		mrw::can::SwitchState switch_state = mrw::can::SWITCH_STATE_LEFT;

	public:
		explicit RegularSwitchWidget(QWidget * parent = nullptr);

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
