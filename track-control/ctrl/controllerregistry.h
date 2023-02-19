//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_CONTROLLERREGISTRY_H
#define MRW_CTRL_CONTROLLERREGISTRY_H

#include <unordered_map>

#include <QObject>

#include <can/mrwbusservice.h>
#include <model/device.h>
#include <ctrl/controllerregistrand.h>
#include <ctrl/batch.h>
#include <util/singleton.h>

namespace mrw::ctrl
{
	class ControllerRegistrand;

	class ControllerRegistry :
		public QObject,
		public mrw::util::Singleton<ControllerRegistry>
	{
		Q_OBJECT

	private:
		ControllerRegistry();
		~ControllerRegistry();

		friend class Singleton<ControllerRegistry>;

		std::unordered_map<mrw::model::Device *, ControllerRegistrand * >  registry;
		mrw::can::MrwBusService                      *                     can_service = nullptr;

	public:
		void registerController(
			mrw::model::Device   *   device,
			ControllerRegistrand  *  ctrl);
		void unregisterController(mrw::model::Device * device);

		ControllerRegistrand * find(model::Device * device) const;

		template <class R> R * find(mrw::model::Device * device) const
		{
			return dynamic_cast<R *>(find(device));
		}

		template <class R> void collect(std::vector<R *> & collection)
		{
			for (const auto & it : registry)
			{
				R * element = dynamic_cast<R *>(it.second);

				if (element != nullptr)
				{
					collection.push_back(element);
				}
			}
		}

		void registerService(mrw::can::MrwBusService * service);
		static mrw::can::MrwBusService * can();

	signals:
		void clear();
		void start();
		void failed();
	};
}

#endif
