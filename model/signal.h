//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SIGNAL_H
#define MRW_MODEL_SIGNAL_H

#include "model/module.h"
#include "model/assemblypart.h"

namespace mrw::model
{
	class Signal : public AssemblyPart
	{
	protected:
		const bool       direction;
		const UnitNo     unit_no;
		const bool       is_main_signal;

	public:
		explicit Signal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const bool            is_main);

		inline UnitNo id() const
		{
			return unit_no;
		}
	};
}

#endif // MRW_MODEL_SIGNAL_H
