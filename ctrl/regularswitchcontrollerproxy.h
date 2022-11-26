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

namespace mrw::ctrl
{
	class RegularSwitchControllerProxy :
		public RegularSwitchController,
		public ControllerRegistrand
	{
		Q_OBJECT

	private:
		mrw::model::RegularSwitch * part = nullptr;

	public:
		explicit RegularSwitchControllerProxy(
			mrw::model::RegularSwitch * new_part,
			QObject          *          parent = nullptr);
		virtual ~RegularSwitchControllerProxy();

		// Implementations from BaseController
		virtual QString name() const override;
		virtual bool    isDirection() const override;

		virtual mrw::model::Position     *    position() const override;
		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from RegularSwitchController
		virtual bool isLeft() const override;
		virtual bool isRight() const override;
		virtual bool isRightBended() const override;
		virtual bool isInclined() const override;
	};
}

#endif
