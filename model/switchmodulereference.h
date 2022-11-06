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

	class SwitchModuleReference : public Device
	{
		static const std::regex  path_regex;

		Controller  *  switch_controller = nullptr;
		SwitchModule * switch_module     = nullptr;

	public:
		explicit SwitchModuleReference(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		inline Controller * controller() const override
		{
			return switch_controller;
		}

		inline SwitchModule * module() const
		{
			return switch_module;
		}
	};
}

#endif
