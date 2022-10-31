//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_SWITCHMODULEREFERENCE_H
#define MRW_MODEL_SWITCHMODULEREFERENCE_H

#include <regex>

#include <QDomElement>

namespace mrw::model
{
	class ModelRailway;
	class SwitchModule;

	class SwitchModuleReference
	{
		static const std::regex  path_regex;

		SwitchModule * switch_module = nullptr;

	public:
		explicit SwitchModuleReference(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);

		inline SwitchModule * module() const
		{
			return switch_module;
		}
	};
}

#endif
