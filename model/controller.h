//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_CONTROLLER_H
#define MRW_MODEL_CONTROLLER_H

#include <cinttypes>
#include <vector>

#include <QDomElement>

#include "model/module.h"

namespace mrw::model
{
	typedef uint16_t ControllerId;

	class ModelRailway;
	class MultiplexConnection;

	class Controller
	{
		const ControllerId  controller_id;
		ModelRailway    *   model = nullptr;
		std::vector<Module *>               modules;
		std::vector<MultiplexConnection *>  connections;

	public:
		explicit Controller(
			ModelRailway * model_railway,
			const QDomElement & element);
		virtual ~Controller();

		void add(Module * module);
		void add(MultiplexConnection * connection);
		void link();

		inline ControllerId id() const
		{
			return controller_id;
		}

		inline Module * module(const ModuleId index) const
		{
			return modules.at(index);
		}

		inline MultiplexConnection * connection(const ModuleId index) const
		{
			return connections.at(index);
		}
	};
}

#endif // CONTROLLER_H
