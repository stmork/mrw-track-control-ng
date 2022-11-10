//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SWITCHMODULEREFERENCE_H
#define MRW_MODEL_SWITCHMODULEREFERENCE_H

#include <regex>

#include <QDomElement>

#include <model/device.h>

namespace mrw::model
{
	class ModelRailway;
	class Controller;
	class SwitchModule;

	/**
	 * This base class represents the connection of an inductive actuator
	 * Device to the corresponding SwitchModule. An inductive actuator may
	 * be one of the following Devices:
	 * - A form signal (FormSignal)
	 * - A simple switch (RailSwitch)
	 * - A cross double switch (RailDoubleSwitch)
	 */
	class SwitchModuleReference : public Device
	{
		static const std::regex  path_regex;

		const unsigned inductor_count;
		const bool     has_cut_off;

		Controller  *  switch_controller = nullptr;
		SwitchModule * switch_module     = nullptr;

	public:
		explicit SwitchModuleReference(
			ModelRailway     *    model_railway,
			const QDomElement  &  element,
			const bool            cutoff = true);

		/**
		 * This method returns how many inductive actuators this Device
		 * contain. Normal FormSignals and RailSwitches need only two
		 * actuators where as a FormSignal having aspect Hp2 needs three
		 * actuators.
		 *
		 * @return The number of inductive actuators needed by this Device.
		 */
		inline size_t inductors() const
		{
			return inductor_count;
		}

		/**
		 * This method returns true if the inductive actuators have automatic
		 * cutoff at end position.
		 *
		 * @return True on automatic end position cutoff.
		 */
		inline bool hasCutOff() const
		{
			return has_cut_off;
		}

		/**
		 * This method returns the controlling SwitchModule.
		 *
		 * @return The controlling SwitchModule.
		 */
		inline SwitchModule * module() const
		{
			return switch_module;
		}

		/**
		 * This method returns the CAN Controller to which the controlling
		 * SwitchModule is connected to.
		 *
		 * @return The CAN Controller of the controlling SwitchModule.
		 */
		inline Controller * controller() const override
		{
			return switch_controller;
		}
	};
}

#endif
