//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_DEVICE_H
#define MRW_MODEL_DEVICE_H

#include <model/module.h>

namespace mrw::model
{

	class Device
	{
		const UnitNo             unit_no;

	public:
		Device(const QDomElement  &  element);

		inline UnitNo id() const
		{
			return unit_no;
		}
	};
}

#endif
