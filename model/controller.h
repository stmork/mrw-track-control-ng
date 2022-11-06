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

#include <util/stringutil.h>
#include <model/module.h>

namespace mrw::model
{
	typedef uint16_t ControllerId;

	class ModelRailway;
	class MultiplexConnection;

	class Controller : public mrw::util::String
	{
		const ControllerId                  controller_id;
		ModelRailway            *           model = nullptr;
		std::vector<Module *>               modules;
		std::vector<MultiplexConnection *>  connections;

	public:
		explicit Controller(
			ModelRailway * model_railway,
			const QDomElement & element);
		virtual ~Controller();

		void link();

		inline ControllerId id() const
		{
			return controller_id;
		}

		inline Module * module(const int index) const
		{
			return modules.at(index);
		}

		inline size_t moduleCount() const
		{
			return modules.size();
		}

		inline MultiplexConnection * connection(const ModuleId index) const
		{
			return connections.at(index);
		}

		virtual QString toString() const override;
	};
}

#endif // CONTROLLER_H
