//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLERPROXY_H
#define MRW_CTRL_REGULARSWITCHCONTROLLERPROXY_H

#include <model/regularswitch.h>
#include <ctrl/regularswitchcontroller.h>
#include <ctrl/switchcontroller.h>

namespace mrw::ctrl
{
	class RegularSwitchControllerProxy :
		public RegularSwitchController,
		public SwitchController
	{
		Q_OBJECT

	private:
		mrw::model::RegularSwitch  *  part = nullptr;

	public:
		explicit RegularSwitchControllerProxy(
			mrw::model::RegularSwitch * new_part,
			QObject          *          parent = nullptr);
		virtual ~RegularSwitchControllerProxy();

		void turnLeft();
		void change();
		void turnRight();

	signals:
		void turn();
		void leftResponse();
		void rightResponse();
		void failed();
		void start();
		void unlock();
		void stop();

	private:

		// Implementations from BaseController
		virtual const QString & name()         const noexcept override;
		virtual bool            isDirection()  const noexcept override;
		virtual float           extensions()   const noexcept override;

		virtual mrw::model::Position     *    position() const noexcept override;
		virtual mrw::model::SectionState      state()    const noexcept override;
		virtual mrw::model::Device::LockState lock()     const noexcept override;
		virtual mrw::model::Position::Bending bending()  const noexcept override;

		// Implementations from BaseSwitchController
		virtual bool isRightBended() const override;
		virtual bool hasFlankProtection() const override;

		// Implementations from RegularSwitchController
		virtual bool isLeft() const override;
		virtual bool isRight() const override;
		virtual bool isInclined() const override;

		// Implementation from RailPartInfo
		virtual mrw::model::RailPart * railPart() const override;

		// Implementations from ControllerRegistrand
		virtual bool    process(const can::MrwMessage & message) override;
		virtual void    restart() override;
		virtual QString toString() const override;

		// Implementations from OperationCallback
		virtual void left() override;
		virtual void right() override;
		virtual void request() override;
		virtual bool doTurnLeft() override;
		virtual bool isFree() override;

		virtual void fail() override;
		virtual void pending() override;
		virtual void lock(bool do_it) override;
	};
}

#endif
