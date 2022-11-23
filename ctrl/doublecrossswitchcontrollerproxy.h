//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_DOUBLECROSSSWITCHCONTROLLERPROXY_H
#define MRW_CTRL_DOUBLECROSSSWITCHCONTROLLERPROXY_H

#include <ctrl/doublecrossswitchcontroller.h>
#include <model/doublecrossswitch.h>

namespace mrw::ctrl
{
	class DoubleCrossSwitchControllerProxy : public DoubleCrossSwitchController
	{
		Q_OBJECT

	private:
		mrw::model::DoubleCrossSwitch * part = nullptr;

	public:
		explicit DoubleCrossSwitchControllerProxy(
			mrw::model::DoubleCrossSwitch * new_part,
			QObject            *            parent = nullptr);

		void setSwitch(mrw::model::DoubleCrossSwitch * new_part);

		virtual bool isDirection() const override;
		virtual QString name() const override;

		virtual mrw::model::Position * position() const override
		{
			return part;
		}

		virtual mrw::model::SectionState state() const override
		{
			return part->section()->state();
		}

		virtual mrw::model::DoubleCrossSwitch::State switchState() const override
		{
			return part->state();
		}

		virtual mrw::model::Device::LockState lock() const override
		{
			return part->lock();
		}
	};
}

#endif
