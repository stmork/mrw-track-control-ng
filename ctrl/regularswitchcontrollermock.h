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
		bool                  inclined     = false;
		bool                  a_is_dir     = false;

		mrw::model::Device::LockState     lock_state =
			mrw::model::Device::LockState::UNLOCKED;
		mrw::model::SectionState          section_state =
			mrw::model::SectionState::FREE;
		mrw::model::RegularSwitch::State  switch_state =
			mrw::model::RegularSwitch::State::AB;

	public:
		RegularSwitchControllerMock(QObject * parent = nullptr);

		virtual bool    isLeft() const override;
		virtual bool    isRight() const override;
		virtual bool    isRightBended() const override;
		virtual bool    isDirection() const override;
		virtual bool    isInclined() const override;
		virtual QString name() const override;

		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		void setDirection(const bool dir = true);
		void setSectionState(const mrw::model::SectionState state);
		void setLock(const mrw::model::Device::LockState lock);

	signals:
		void computeConnectors();

	public slots:
		void setLeft(const bool left = true);
		void setRight(const bool right = true);
		void setLeftHanded(const bool left = true);
		void setRightHanded(const bool right = true);
		void setInclined(const bool inclined = true);
	};
}

#endif
