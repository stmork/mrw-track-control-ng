//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_CONTROLLER_H
#define MRW_MODEL_CONTROLLER_H

#include <cinttypes>
#include <vector>

#include <QDomElement>

#include <util/stringutil.h>
#include <can/types.h>
#include <model/module.h>
#include <model/multiplexconnection.h>

namespace mrw::model
{
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
	 * - section module (SectionModule)
	 * - switch module (SwitchModule)
	 * - light module (LightModule)
	 *
	 * The Module elements and serial MultiplexConnection elements do not
	 * have any intelligence. They are controlled from this controller and
	 * the behaviour is depending on commands sent vie CAN bus to the
	 * controller.
	 */
	class Controller : public mrw::util::String
	{
		const mrw::can::ControllerId        controller_id;
		ModelRailway            *           model = nullptr;
		std::vector<Module *>               modules;
		std::vector<MultiplexConnection *>  connections;

		friend class ModelRailway;

		static constexpr size_t MAX_PORTS      = 3;
		static constexpr size_t MAX_CONNECTORS = 4;

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
		mrw::can::ControllerId id() const noexcept;

		/**
		 * This method returns the nth Module element. It is not ID-based
		 * but index based and is used for linking the modules after the
		 * model has loaded into memory.
		 *
		 * @param index The zero based index of connected Module boards.
		 * @return The found Module instance.
		 */
		Module * module(const int index) const;

		/**
		 * This method returns the amount of connected Module elements.
		 *
		 * @return The connected Module count.
		 */
		size_t moduleCount() const noexcept;

		/**
		 * This method returns the nth MultiplexConnection. It is not ID-based
		 * but index based and is used for linking the connections after the
		 * model is loaded into memory.
		 *
		 * @param index The zero based index of connected MultiplexConnection
		 * boards.
		 * @return The found MultiplexConnection instance.
		 */
		MultiplexConnection * connection(const ModuleId index) const;

		/**
		 * This method returns the amount of connected serial multiplex
		 * connection modules.
		 *
		 * @return The connected MultiplexConnection count.
		 */
		size_t connectionCount() const noexcept;

		/**
		 * This method validates if all underlying modules and connections
		 * are also valid.
		 *
		 * @return True if every configuration is fine.
		 * @see Module::valid()
		 * @see MultiplexConnection::valid()
		 */
		bool valid() const noexcept;

		/**
		 * This method collects all configuration messages which describes
		 * the connected Device instances of this Controller.
		 *
		 * @note The vector contains no beginning CFGBGN nor ending CFGEND
		 * MrwMessage.
		 *
		 * @param messages The mrw::can::MrwMessage vector to collect the
		 * configuration messages.
		 */
		void configure(std::vector<mrw::can::MrwMessage> & messages) const;

		virtual QString toString() const override;

	private:
		/**
		 * This method links all elements needed for the implementation.
		 */
		void   link();
		size_t ports() const;
	};
}

#endif
