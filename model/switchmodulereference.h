//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
	 * - A regular switch (RegularSwitch)
	 * - A double cross switch (DoubleCrossSwitch)
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
		 * contain. Normal FormSignal and AbstractSwitch parts need only two
		 * actuators where as a FormSignal having aspect Hp2 needs three
		 * actuators.
		 *
		 * @return The number of inductive actuators needed by this Device.
		 */
		size_t inductors() const noexcept;

		/**
		 * This method returns true if the inductive actuators have automatic
		 * cutoff at end position.
		 *
		 * @return True on automatic end position cutoff.
		 */
		bool hasCutOff() const noexcept;

		/**
		 * This method returns the controlling SwitchModule.
		 *
		 * @return The controlling SwitchModule.
		 */
		SwitchModule * module() const noexcept;

		Controller * controller() const noexcept override;

	protected:
		mrw::can::MrwMessage configSwitchMsg(
			const mrw::can::Command cmd,
			const unsigned          pin) const noexcept;
	};
}

#endif
