//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_MODULE_H
#define MRW_MODEL_MODULE_H

#include <cinttypes>
#include <vector>

#include <QDomElement>

#include <can/mrwmessage.h>

namespace mrw::model
{
	typedef uint16_t ModuleId;

	class ModelRailway;
	class AssemblyPart;

	/**
	 * This base class represents all modules which are connected to the
	 * CAN Controller using the 16-pin connector. Actually there are three
	 * different module:
	 * - A section module (SectionModule) to control Section parts.
	 * - A switch module (SwitchModule) to control switches and FormSignal
	 * parts.
	 * - A light module (LightModule) to control ProfileLight parts.
	 */
	class Module
	{
		friend class Controller;

		const ModuleId      module_id;

	protected:
		/**
		 * The pointer to the top level model root element needed for resolving
		 * other components.
		 */
		ModelRailway    *   model = nullptr;

		/**
		 * The XML-DOM element representing this instance. It contains the
		 * path top down to another element to be resolved.
		 */
		const QDomElement   reference;

	public:
		/** The maximum pins per port. */
		static const size_t MAX_PINS_PER_PORT = 8;

		explicit Module(
			ModelRailway    *   model_railway,
			const QDomElement & element);
		virtual ~Module() = default;

		/**
		 * This method returns the module ID. This ID is not used to by
		 * any CAN command.
		 *
		 * @return The modules' ID.
		 */
		inline ModuleId id() const
		{
			return module_id;
		}

		/**
		 * This method returns the needed amount of 16-Bit Ports of the CAN
		 * controller.
		 *
		 * @return The needed 16-Bit ports of the CAN controller.
		 */
		virtual size_t ports() const = 0;

		/**
		 * This method verifies if this Module is not overprovisioned.
		 *
		 * @return True if all pins/connectors satifies the connected
		 * Device elements.
		 */
		virtual bool valid() const = 0;

	protected:

		/**
		 * This method collects all configuration messages which describes
		 * the connected Device instances.
		 *
		 * @param messages The mrw::canMrwMessage vector to collect the
		 * configuration messages.
		 * @param offset The offset in pin count.
		 * @see MAX_PINS_PER_PORT
		 */
		virtual void configure(
			std::vector<mrw::can::MrwMessage> & messages,
			const unsigned                      offset) const = 0;

	private:
		virtual void link() = 0;
	};
}

#endif
