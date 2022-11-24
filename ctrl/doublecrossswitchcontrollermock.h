//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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
		bool                                 direction    = false;

		mrw::model::Device::LockState        lock_state =
			mrw::model::Device::LockState::UNLOCKED;
		mrw::model::SectionState             section_state =
			mrw::model::SectionState::FREE;
		mrw::model::DoubleCrossSwitch::State switch_state =
			mrw::model::DoubleCrossSwitch::State::AC;

	public:
		DoubleCrossSwitchControllerMock(QObject * parent = nullptr);

		virtual bool    isDirection() const override;

		virtual QString name() const override;
		virtual mrw::model::SectionState state() const override
		{
			return section_state;
		}

		virtual mrw::model::DoubleCrossSwitch::State switchState() const override
		{
			return switch_state;
		}

		virtual mrw::model::Device::LockState lock() const override
		{
			return lock_state;
		}

		virtual mrw::model::Position::Curve curve() const override;

		void setSectionState(const mrw::model::SectionState state);
		void setSwitchState(const mrw::model::DoubleCrossSwitch::State state);
		void setDirection(const bool dir = true);
		void setLock(const mrw::model::Device::LockState lock);

	signals:
		void computeConnectors();
	};
}

#endif
