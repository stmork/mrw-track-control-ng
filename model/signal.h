//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SIGNAL_H
#define MRW_MODEL_SIGNAL_H

#include "model/module.h"
#include "model/assemblypart.h"
#include <model/device.h>

namespace mrw::model
{
	class Signal :
		public AssemblyPart,
		public Device
	{
	protected:
		const bool       direction;
		const bool       is_main_signal;

	public:
		explicit Signal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const bool            is_main);
	};
}

#endif // MRW_MODEL_SIGNAL_H
