//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_CONTROLLERREGISTRY_H
#define MRW_CTRL_CONTROLLERREGISTRY_H

#include <unordered_set>
#include <unordered_map>

#include <QObject>

#include <can/mrwbusservice.h>
#include <model/device.h>
#include <ctrl/controllerregistrand.h>
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

		std::unordered_set<ControllerRegistrand *>                         transaction;
		std::unordered_map<mrw::model::Device *, ControllerRegistrand * >  registry;
		mrw::can::MrwBusService                      *                     can_service = nullptr;

	public:
		void registerController(
			mrw::model::Device   *   device,
			ControllerRegistrand  *  ctrl);
		void unregisterController(mrw::model::Device * device);

		ControllerRegistrand * find(mrw::model::Device * device) const;

		template <class R> R * find(mrw::model::Device * device) const
		{
			return dynamic_cast<R *>(find(device));
		}

		void registerService(mrw::can::MrwBusService * service);
		static mrw::can::MrwBusService * can();

		void increase(ControllerRegistrand * ctrl);
		void decrease(ControllerRegistrand * ctrl);
		bool contains(ControllerRegistrand * ctrl);
		void reset();
		void complete();
		void dump();

	signals:
		void clear();
		void start();
		void completed();
		void failed();
	};
}

#endif
