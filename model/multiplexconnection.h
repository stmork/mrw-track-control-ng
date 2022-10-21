//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODULE_MULTIPLEXCONNECTION_H
#define MRW_MODULE_MULTIPLEXCONNECTION_H

#include <vector>

#include "model/module.h"

namespace mrw::model
{
	class LightSignal;

	class MultiplexConnection
	{
		ModelRailway    *   model = nullptr;

		const QDomElement   reference;
		const ModuleId      connection_id;

		std::vector<LightSignal *> light_signals;

	public:
		explicit MultiplexConnection(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		inline ModuleId id() const
		{
			return connection_id;
		}

		void link();
	};
}

#endif
