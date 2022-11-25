//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_REGULARSWITCHCONTROLLER_H
#define MRW_CTRL_REGULARSWITCHCONTROLLER_H

#include <ctrl/basecontroller.h>

namespace mrw::ctrl
{
	class RegularSwitchController : public BaseController
	{
		Q_OBJECT

	public:
		explicit RegularSwitchController(QObject * parent = nullptr);

		virtual bool  isLeft() const = 0;
		virtual bool  isRight() const = 0;
		virtual bool  isRightBended() const = 0;
		virtual bool  isInclined() const = 0;
		virtual float extensions() const override;
	};
}

#endif
