//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_CONTROLLERREGISTRY_H
#define MRW_CTRL_CONTROLLERREGISTRY_H

#include <unordered_map>

#include <model/device.h>
#include <ctrl/controllerregistrand.h>
#include <util/singleton.h>

namespace mrw::ctrl
{
	class ControllerRegistrand;

	class ControllerRegistry :
		public mrw::util::Singleton<ControllerRegistry>
	{
		ControllerRegistry() = default;
		~ControllerRegistry();

		friend class Singleton<ControllerRegistry>;

		std::unordered_map<mrw::model::Device *, ControllerRegistrand * > registry;

	public:
		void registerController(
			mrw::model::Device   *   device,
			ControllerRegistrand  *  ctrl);
		void unregisterController(mrw::model::Device * device);
		ControllerRegistrand * find(mrw::model::Device * device);
	};
}

#endif
