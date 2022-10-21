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
	typedef unsigned UnitNo;

	class ModelRailway;
	class RailPart;

	class Module
	{
		const ModuleId      module_id;

	protected:
		ModelRailway    *   model = nullptr;
		const QDomElement   reference;

	public:
		explicit Module(
			ModelRailway    *   model_railway,
			const QDomElement & element);
		virtual ~Module() = default;

		inline ModuleId id() const
		{
			return module_id;
		}

		virtual void link() = 0;
	};
}

#endif
