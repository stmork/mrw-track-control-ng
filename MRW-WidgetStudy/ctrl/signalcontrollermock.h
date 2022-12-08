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
		unsigned  extension      = 0;
		bool      direction      = true;
		bool      shunting       = false;
		bool      distant_signal = false;
		bool      main_signal    = true;

		mrw::model::Signal::Symbol shunt_state    = mrw::model::Signal::Symbol::STOP;
		mrw::model::Signal::Symbol distant_state  = mrw::model::Signal::Symbol::STOP;
		mrw::model::Signal::Symbol main_state     = mrw::model::Signal::Symbol::STOP;

		mrw::model::Device::LockState lock_state    =
			mrw::model::Device::LockState::UNLOCKED;
		mrw::model::SectionState      section_state =
			mrw::model::SectionState::FREE;
		mrw::model::Position::Bending bending_state =
			mrw::model::Position::Bending::STRAIGHT;

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

		virtual float extensions() const override
		{
			return extension;
		}

		inline mrw::model::Signal::Symbol distant() const override
		{
			return distant_state;
		}

		inline mrw::model::Signal::Symbol shunt() const override
		{
			return shunt_state;
		}

		inline mrw::model::Signal::Symbol main() const override
		{
			return main_state;
		}

		virtual mrw::model::Position::Bending bending() const override
		{
			return bending_state;
		}

		void setDirection(const bool dir = true);
		void setSectionState(const mrw::model::SectionState state);
		void setBending(const mrw::model::Position::Bending bending);

	signals:
		void extend();
		void computeConnectors();

	public slots:
		void setExtension(const int extension);

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
