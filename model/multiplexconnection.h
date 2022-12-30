//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_MULTIPLEXCONNECTION_H
#define MRW_MODEL_MULTIPLEXCONNECTION_H

#include <vector>

#include <model/module.h>

namespace mrw::model
{
	class Light;
	class LightSignal;
	class Controller;

	/**
	 * This class represents a serial multiplex connection module. Since this
	 * is not connected to the 16-pin IO port of the CAN Controller its
	 * connected using the 10-pin serial connector. It is a cost and space
	 * saving method to address up to 64 light sources such as LightSignals.
	 * Espacially an outgoing main light signal takes five pins and an
	 * additional pre light signal may used four pins more.
	 *
	 * @image html mux-connector.jpg width=40%
	 * @see LightSignal
	 * @see Light
	 */
	class MultiplexConnection
	{
		friend class Controller;

		ModelRailway    *   model = nullptr;

		const QDomElement   reference;
		const ModuleId      connection_id;

		std::vector<LightSignal *> light_signals;
		std::vector<Light *>       lights;

	public:
		/** The maximum light count connected to one module. */
		static const size_t MAX_PINS = 16;

		explicit MultiplexConnection(
			ModelRailway     *    model_railway,
			Controller      *     controller,
			const QDomElement  &  element);
		virtual ~MultiplexConnection();

		/**
		 * This method returns the module ID. This ID is not used to by
		 * any CAN command.
		 *
		 * @return The modules' ID.
		 */
		inline ModuleId id() const
		{
			return connection_id;
		}

		/**
		 * This method verifies if the amount of connected pins is modelled
		 * correctly. The maximum amount is MAX_PINS pins.
		 *
		 * @return True if the configuration of this MultiplexConnection is
		 * correct.
		 */
		bool valid() const;

	private:
		void link();
	};
}

#endif
