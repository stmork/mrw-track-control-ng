//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLERPROXY_H
#define MRW_CTRL_REGULARSWITCHCONTROLLERPROXY_H

#include <ctrl/regularswitchcontroller.h>
#include <model/regularswitch.h>

namespace mrw::ctrl
{
	class RegularSwitchControllerProxy : public RegularSwitchController
	{
		Q_OBJECT

	private:
		mrw::model::RegularSwitch * part = nullptr;

	public:
		RegularSwitchControllerProxy(QObject * parent = nullptr);

		void setSwitch(mrw::model::RegularSwitch * new_part);
	};
}

#endif
