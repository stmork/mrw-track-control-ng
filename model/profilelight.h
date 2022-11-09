//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_PROFILELIGHT_H
#define MRW_MODEL_PROFILELIGHT_H

#include <model/light.h>

namespace mrw::model
{
	class ProfileLight : public Light
	{
		const unsigned profile;

	public:
		explicit ProfileLight(
			ModelRailway     *    model_railway,
			Controller      *     controller,
			const QDomElement  &  element);
	};
}

#endif
