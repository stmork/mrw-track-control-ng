//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLERPROXY_H
#define MRW_CTRL_REGULARSWITCHCONTROLLERPROXY_H

#include <model/regularswitch.h>
#include <ctrl/regularswitchcontroller.h>
#include <ctrl/controllerregistrand.h>
#include <statecharts/SwitchStatechart.h>

namespace mrw::ctrl
{
	class RegularSwitchControllerProxy :
		public RegularSwitchController,
		public ControllerRegistrand,
		public mrw::statechart::SwitchStatechart::OperationCallback
	{
		Q_OBJECT

	private:
		mrw::statechart::SwitchStatechart   statechart;
		mrw::model::RegularSwitch     *     part = nullptr;

	public:
		explicit RegularSwitchControllerProxy(
			mrw::model::RegularSwitch * new_part,
			QObject          *          parent = nullptr);
		virtual ~RegularSwitchControllerProxy();

		void turnLeft();
		void turn();
		void turnRight();

		// Implementations from BaseController
		virtual QString name() const override;
		virtual bool    isDirection() const override;
		virtual bool    isExpandable() const override;
		virtual float   extensions() const override;

		virtual mrw::model::Position     *    position() const override;
		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from RegularSwitchController
		virtual bool isLeft() const override;
		virtual bool isRight() const override;
		virtual bool isRightBended() const override;
		virtual bool isInclined() const override;

		// Implementations from ControllerRegistrand
		virtual bool process(const can::MrwMessage & message) override;

		// Implementations from OperationCallback
		virtual void left() override;
		virtual void right() override;
		virtual void request() override;
		virtual bool isTurnedLeft() override;
		virtual bool isFree() override;

		virtual void pending() override;
		virtual void fail() override;
		virtual void lock(bool do_it) override;
	};
}

#endif
