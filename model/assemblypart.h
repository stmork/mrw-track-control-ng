//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_ASSEMBLYPART_H
#define MRW_MODEL_ASSEMBLYPART_H

#include <regex>

#include <QDomElement>

namespace mrw::model
{
	class ModelRailway;

	class AssemblyPart
	{
		static const std::regex path_regex;

	protected:
		const QString      part_name;
		ModelRailway   *   model = nullptr;
		const QDomElement  reference;

	public:
		explicit AssemblyPart(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~AssemblyPart() = default;

		virtual void link() = 0;

		inline const QString & name() const
		{
			return part_name;
		}

		static AssemblyPart * resolve(
			const ModelRailway * model,
			const QString    &   reference);
	};
}

#endif
