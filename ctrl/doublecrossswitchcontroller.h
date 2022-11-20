//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_DOUBLECROSSSWITCHCONTROLLER_H
#define MRW_CTRL_DOUBLECROSSSWITCHCONTROLLER_H

#include <ctrl/basecontroller.h>
#include <model/doublecrossswitch.h>

namespace mrw::ctrl
{
	class DoubleCrossSwitchController : public BaseController
	{
		Q_OBJECT

	public:
		explicit DoubleCrossSwitchController(QObject * parent = nullptr);

		virtual bool isDirection() const = 0;
		virtual mrw::model::DoubleCrossSwitch::State  switchState() const = 0;
	};
}

#endif
