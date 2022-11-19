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
		DoubleCrossSwitchControllerProxy(QObject * parent = nullptr);

		void setSwitch(mrw::model::DoubleCrossSwitch * new_part);

		virtual bool isDirection() const override;
		virtual QString name() const override;

		virtual mrw::model::SectionState state() const override
		{
			return part->section()->state();
		}
	};
}

#endif
