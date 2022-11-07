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

	class MultiplexConnection;
	class ModelRailway;

	/**
	 * This class represents a CAN-Node named as Controller here. It can
	 * connect up to three Module elements using a 16-Pin connector and
	 * a series of up to four multiplex connections for light signals and
	 * simple threshold controlled lights.
	 *
	 * @image html controller.jpg width=40%
	 *
	 * The Module elements may be:
	 * - section module (@see SectionModule)
	 * - switch module (@see SwitchModule)
	 * - light module (@see LightModule)
	 *
	 * The Module elements and serial MultiplexConnection elements do not
	 * have any intelligence. They are controlled from this controller and
	 * the behaviour is depending on commands sent vie CAN bus to the
	 * controller.
	 */
	class Controller : public mrw::util::String
	{
		const ControllerId                  controller_id;
		ModelRailway            *           model = nullptr;
		std::vector<Module *>               modules;
		std::vector<MultiplexConnection *>  connections;

		friend class ModelRailway;


	public:
		explicit Controller(
			ModelRailway * model_railway,
			const QDomElement & element);
		virtual ~Controller();

		/**
		 * This method returns the controller ID which is part of the
		 * CAN ID. It may be used directly as a basic frame format CAN ID
		 * for the controller configuration. In case to control a connected
		 * Device (using a Module or MultiplexConnection) an extended frame
		 * format is used which is a combination of this controller ID and
		 * the Device unit number.
		 *
		 * @return The controllers CAN ID.
		 */
		inline ControllerId id() const
		{
			return controller_id;
		}

		/**
		 * This method returns the nth Module element. It is not ID-based
		 * but index based and is used for linking the modules after the
		 * model has loaded into memory.
		 *
		 * @param index The zero based index of connected Module boards.
		 * @return The found Module instance.
		 *
		 * @see link()
		 */
		inline Module * module(const int index) const
		{
			return modules.at(index);
		}

		/**
		 * This method returns the amount of connected Module elements.
		 *
		 * @return The connected Module count.
		 */
		inline size_t moduleCount() const
		{
			return modules.size();
		}

		/**
		 * This method returns the nth MultiplexConnection. It is not ID-based
		 * but index based and is used for linking the connections after the
		 * model is loaded into memory.
		 *
		 * @param index The zero based index of connected MultiplexConnection
		 * boards.
		 * @return The found MultiplexConnection instance.
		 */
		inline MultiplexConnection * connection(const ModuleId index) const
		{
			return connections.at(index);
		}

		virtual QString toString() const override;

	private:
		/**
		 * This method links all elements needed for the implementation.
		 */
		void link();
	};
}

#endif // CONTROLLER_H
