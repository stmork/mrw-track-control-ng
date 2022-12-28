//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODULE_SWITCHMODULE_H
#define MRW_MODULE_SWITCHMODULE_H

#include <vector>

#include <QDomElement>

#include <model/module.h>

namespace mrw::model
{
	/**
	 * This class represents a switch module which is connected to a CAN
	 * controller using one or two 16-pin connectors depending on an
	 * existing end response is available inside the inductive actuator.
	 *
	 * @note Even the module is called switch module for historical reasons
	 * it can drive also form signals with inductive actuators.
	 *
	 * @image html switch-module.jpg width=40%
	 *
	 * A section module can control up to four switches. It can be mixed with
	 * form signals which may need three control lines for the inductive
	 * actuators.
	 */
	class SwitchModule : public Module
	{
		std::vector<AssemblyPart *> rail_parts;

	public:
		explicit SwitchModule(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		size_t ports() const override;

		bool valid() const override;

	private:
		void link() override;
	};
}

#endif
