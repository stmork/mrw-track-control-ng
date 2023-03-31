//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
		mutable   QString   signal_name;

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

		void reset();

		void setDirection(const bool dir = true);
		void setSectionState(const mrw::model::SectionState state);
		void setLock(const mrw::model::Device::LockState lock);
		void setBending(const mrw::model::Position::Bending bending);

		void setMainSymbol(const mrw::model::Signal::Symbol symbol);
		void setDistantSymbol(const mrw::model::Signal::Symbol symbol);
		void setShuntSymbol(const mrw::model::Signal::Symbol symbol);

	private:

		// Implementations from BaseController
		virtual const QString & name() const override;
		virtual float           extensions() const override;
		virtual bool            isDirection() const override;

		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from SignalController
		virtual bool hasMain()     const override;
		virtual bool hasDistant()  const override;
		virtual bool hasShunting() const override;

		virtual mrw::model::Signal::Symbol main()    const override;
		virtual mrw::model::Signal::Symbol distant() const override;
		virtual mrw::model::Signal::Symbol shunt()   const override;

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
