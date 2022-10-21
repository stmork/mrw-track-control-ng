//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAILPART_H
#define MRW_MODEL_RAILPART_H

#include <regex>

#include <QDomElement>

namespace mrw::model
{
	class ModelRailway;

	class RailPart
	{
		static const std::regex path_regex;

		const QString      name;

	protected:
		ModelRailway   *   model = nullptr;
		const QDomElement  reference;

	public:
		explicit RailPart(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~RailPart() = default;

		virtual void link() = 0;

		static RailPart * resolve(
			const ModelRailway * model,
			const QString    &   reference);

	protected:
		RailPart * resolve(const char * attr) const;
	};
}

#endif
