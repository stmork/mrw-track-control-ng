//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_FORMSIGNAL_H
#define MRW_MODEL_FORMSIGNAL_H

#include <regex>

#include "model/signal.h"
#include "model/switchmodulereference.h"

namespace mrw::model
{
	class FormSignal : public Signal, public SwitchModuleReference
	{
		const unsigned inductors;

	public:
		explicit FormSignal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const bool            is_main);

		void link() override;
	};
}

#endif // MRW_MODEL_SIGNAL_H
