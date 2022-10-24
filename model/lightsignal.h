//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_LIGHTSIGNAL_H
#define MRW_MODEL_LIGHTSIGNAL_H

#include <regex>

#include "model/module.h"
#include "model/signal.h"

namespace mrw::model
{
	class MultiplexConnection;

	class LightSignal : public Signal
	{
		static const std::regex path_regex;

		const unsigned lights;

		MultiplexConnection * connection = nullptr;

	public:
		explicit LightSignal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const bool            is_main,
			const unsigned        light_count);

		void link() override;
	};
}

#endif // MRW_MODEL_SIGNAL_H
