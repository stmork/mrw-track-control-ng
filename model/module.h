//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODULE_MODULE_H
#define MRW_MODULE_MODULE_H

#include <cinttypes>

#include <QDomElement>

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
		 * This method verifies if this Module is not overprovisioned.
		 *
		 * @return True if all pins/connectors satifies the connected
		 * Device elements.
		 */
		virtual bool valid() const = 0;

	private:
		virtual void link() = 0;
	};
}

#endif
