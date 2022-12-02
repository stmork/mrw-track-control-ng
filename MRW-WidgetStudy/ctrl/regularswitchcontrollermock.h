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
		bool                  inclined     = false;
		bool                  a_is_dir     = false;
		unsigned              extension    = 0;
		bool                  left_handed  = true;

		mrw::model::Device::LockState     lock_state =
			mrw::model::Device::LockState::UNLOCKED;
		mrw::model::SectionState          section_state =
			mrw::model::SectionState::FREE;
		mrw::model::RegularSwitch::State  switch_state =
			mrw::model::RegularSwitch::State::AB;

	public:
		RegularSwitchControllerMock(QObject * parent = nullptr);

		// Implementations from BaseController
		virtual QString name() const override;
		virtual bool    isDirection() const override;
		virtual float   extensions() const override;

		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from RegularSwitchController
		virtual bool    isLeft() const override;
		virtual bool    isRight() const override;
		virtual bool    isRightBended() const override;
		virtual bool    isInclined() const override;

		void setDirection(const bool dir = true);
		void setSectionState(const mrw::model::SectionState state);
		void setLock(const mrw::model::Device::LockState lock);

	signals:
		void extend();
		void computeConnectors();

	public slots:
		void setLeft(const bool left = true);
		void setRight(const bool right = true);
		void setLeftHanded(const bool left = true);
		void setRightHanded(const bool right = true);
		void setInclined(const bool inclined = true);
		void setExtension(const int extension);
	};
}

#endif
