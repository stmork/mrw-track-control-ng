//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_CONTROLLERREGISTRAND_H
#define MRW_CTRL_CONTROLLERREGISTRAND_H

#include <util/stringutil.h>
#include <can/mrwmessage.h>

namespace mrw::ctrl
{
	class ControllerRegistrand : public mrw::util::String
	{
	public:
		ControllerRegistrand();

		virtual bool process(const can::MrwMessage & message);
	};
}

#endif
