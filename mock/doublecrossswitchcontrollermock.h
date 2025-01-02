//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_DOUBLECROSSSWITCHCONTROLLERMOCK_H
#define MRW_CTRL_DOUBLECROSSSWITCHCONTROLLERMOCK_H

#include <can/commands.h>
#include <ctrl/doublecrossswitchcontroller.h>

namespace mrw::ctrl
{
	class DoubleCrossSwitchControllerMock : public DoubleCrossSwitchController
	{
		Q_OBJECT

	private:
		bool                                 a_is_dir         = false;
		bool                                 left_handed      = true;
		bool                                 flank_protection = false;

		mrw::model::Device::LockState        lock_state =
			mrw::model::Device::LockState::UNLOCKED;
		mrw::model::SectionState             section_state =
			mrw::model::SectionState::FREE;
		mrw::model::DoubleCrossSwitch::State switch_state =
			mrw::model::DoubleCrossSwitch::State::AC;

	public:
		DoubleCrossSwitchControllerMock(QObject * parent = nullptr);

		void reset();

		void setSectionState(const mrw::model::SectionState state);
		void setSwitchState(const mrw::model::DoubleCrossSwitch::State state);
		void setDirection(const bool dir = true);
		void setLock(const mrw::model::Device::LockState lock);

	private:

		// Implementations from BaseController
		virtual const QString & name()        const override;
		virtual bool            isDirection() const override;

		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from BaseSwitchController
		virtual bool isRightBended() const override;
		virtual bool hasFlankProtection() const override;

		// Implementations from DoubleCrossSwitchController
		virtual mrw::model::DoubleCrossSwitch::State switchState() const override;

	public slots:
		void setLeftHanded(const bool left = true);
		void setRightHanded(const bool right = true);
		void setFlankProtection(const bool flank = false);

	signals:
		void computeConnectors();
	};
}

#endif
