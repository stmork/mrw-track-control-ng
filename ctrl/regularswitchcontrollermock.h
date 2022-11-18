//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLERMOCK_H
#define MRW_CTRL_REGULARSWITCHCONTROLLERMOCK_H

#include <can/commands.h>
#include <ctrl/regularswitchcontroller.h>

namespace mrw::ctrl
{
	class RegularSwitchControllerMock :
		public mrw::ctrl::RegularSwitchController
	{
		Q_OBJECT

	private:
		bool                  left_handed  = true;
		bool                  direction    = false;
		mrw::can::SwitchState switch_state = mrw::can::SWITCH_STATE_LEFT;

	public:
		RegularSwitchControllerMock(QObject * parent = nullptr);

		virtual bool isLeft() const override;
		virtual bool isRight() const override;
		virtual bool isRightHanded() const override;
		virtual bool isDirection() const override;
		virtual QString name() const override;

	public slots:
		void setLeft(const bool left = true);
		void setRight(const bool right = true);
		void setLeftHanded(const bool left = true);
		void setRightHanded(const bool right = true);
		void setDirection(const bool dir = true);
	};
}

#endif
