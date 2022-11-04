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
	public:
		enum SignalType
		{
			MAIN_SIGNAL,
			PRE_SIGNAL,
			SHUNT_SIGNAL
		};

	protected:
		const bool          direction;
		const SignalType    signal_type;

	public:
		explicit Signal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const SignalType      type);

		QString toString() const override;
	};
}

#endif // MRW_MODEL_SIGNAL_H
