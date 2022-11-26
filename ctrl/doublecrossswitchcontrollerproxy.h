//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_DOUBLECROSSSWITCHCONTROLLERPROXY_H
#define MRW_CTRL_DOUBLECROSSSWITCHCONTROLLERPROXY_H

#include <model/doublecrossswitch.h>
#include <ctrl/doublecrossswitchcontroller.h>
#include <ctrl/controllerregistrand.h>

namespace mrw::ctrl
{
	class DoubleCrossSwitchControllerProxy :
		public DoubleCrossSwitchController,
		public ControllerRegistrand
	{
		Q_OBJECT

	private:
		mrw::model::DoubleCrossSwitch * part = nullptr;

	public:
		explicit DoubleCrossSwitchControllerProxy(
			mrw::model::DoubleCrossSwitch * new_part,
			QObject            *            parent = nullptr);
		virtual ~DoubleCrossSwitchControllerProxy();

		void setSwitch(mrw::model::DoubleCrossSwitch * new_part);

		// Implementations from BaseController
		virtual QString name() const override;
		virtual bool    isDirection() const override;

		virtual mrw::model::Position     *    position() const override;
		virtual mrw::model::SectionState      state() const override;
		virtual mrw::model::Device::LockState lock() const override;
		virtual mrw::model::Position::Bending bending() const override;

		// Implementations from DoubleCrossSwitchController
		virtual mrw::model::DoubleCrossSwitch::State switchState() const override;
	};
}

#endif
