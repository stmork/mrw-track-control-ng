//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_LIGHTSIGNAL_H
#define MRW_MODEL_LIGHTSIGNAL_H

#include <regex>

#include <model/module.h>
#include <model/signal.h>
#include <model/device.h>

namespace mrw::model
{
	class MultiplexConnection;

	class LightSignal : public Signal, public Device
	{
		static const std::regex path_regex;

		const unsigned lights;

		Controller      *     signal_controller = nullptr;
		MultiplexConnection * mux_connection    = nullptr;

	public:
		explicit LightSignal(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const SignalType      type,
			const unsigned        light_count);

		inline Controller * controller() const override
		{
			return signal_controller;
		}

		inline MultiplexConnection * connection() const
		{
			return mux_connection;
		}

		inline const QString & name() const override
		{
			return part_name;
		}
	};
}

#endif // MRW_MODEL_SIGNAL_H
