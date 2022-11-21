//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_SIGNALCONTROLLERMOCK_H
#define MRW_CTRL_SIGNALCONTROLLERMOCK_H

#include <ctrl/signalcontroller.h>

namespace mrw::ctrl
{
	class SignalControllerMock : public SignalController
	{
		Q_OBJECT

	private:
		bool direction      = true;
		bool shunting       = false;
		bool distant_signal = false;
		bool main_signal    = true;

		TourState shunt_state   = TourState::STOP;
		TourState distant_state = TourState::STOP;
		TourState main_state    = TourState::STOP;
		mrw::model::Device::LockState lock_state    = mrw::model::Device::LockState::UNLOCKED;

		mrw::model::SectionState section_state = mrw::model::SectionState::FREE;

	public:
		explicit SignalControllerMock(QObject * parent = nullptr);

		virtual bool isDirection() const override;
		virtual bool hasShunting() const override;
		virtual bool hasDistant() const override;
		virtual bool hasMain() const override;
		virtual QString name() const override;

		virtual mrw::model::SectionState state() const override
		{
			return section_state;
		}

		virtual mrw::model::Device::LockState lock() const override
		{
			return lock_state;
		}

		inline TourState distant() const override
		{
			return distant_state;
		}

		inline TourState shunt() const override
		{
			return shunt_state;
		}

		inline TourState main() const override
		{
			return main_state;
		}

		void setDirection(const bool dir = true);
		void setSectionState(const mrw::model::SectionState state);

	public slots:
		void setShuntStop();
		void setShuntGo();
		void setDistantStop();
		void setDistantGo();
		void setMainStop();
		void setMainGo();

		void setShunting(const bool shunt = true);
		void setDistant(const bool distant = true);
		void setMain(const bool main = true);
	};
}

#endif // MRW_CTRL_SIGNALCONTROLLERMOCK_H
