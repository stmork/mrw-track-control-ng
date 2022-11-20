//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLERMOCK_H
#define MRW_CTRL_REGULARSWITCHCONTROLLERMOCK_H

#include <model/regularswitch.h>
#include <ctrl/regularswitchcontroller.h>

namespace mrw::ctrl
{
	class RegularSwitchControllerMock : public RegularSwitchController
	{
		Q_OBJECT

	private:
		bool                  left_handed  = true;
		bool                  direction    = false;
		LockState             lock_state   = LockState::UNLOCKED;

		mrw::model::SectionState          section_state = mrw::model::SectionState::FREE;
		mrw::model::RegularSwitch::State  switch_state  = mrw::model::RegularSwitch::State::AB;

	public:
		RegularSwitchControllerMock(QObject * parent = nullptr);

		virtual bool    isLeft() const override;
		virtual bool    isRight() const override;
		virtual bool    isRightHanded() const override;
		virtual bool    isDirection() const override;
		virtual QString name() const override;

		virtual mrw::model::SectionState state() const override
		{
			return section_state;
		}

		virtual LockState lock() const override
		{
			return lock_state;
		}

		void setSectionState(const mrw::model::SectionState state);
		void setDirection(const bool dir = true);
		void setLock(const LockState lock);

	public slots:
		void setLeft(const bool left = true);
		void setRight(const bool right = true);
		void setLeftHanded(const bool left = true);
		void setRightHanded(const bool right = true);
	};
}

#endif
