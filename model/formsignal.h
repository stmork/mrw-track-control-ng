//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_FORMSIGNAL_H
#define MRW_MODEL_FORMSIGNAL_H

#include <regex>

#include "model/module.h"
#include "model/signal.h"

namespace mrw::model
{
	class SwitchModule;

	class FormSignal : public Signal
	{
		static const std::regex path_regex;

		const unsigned inductors;

		SwitchModule * module = nullptr;

	public:
		explicit FormSignal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const bool            is_main);

		void link() override;
	};
}

#endif // MRW_MODEL_SIGNAL_H
