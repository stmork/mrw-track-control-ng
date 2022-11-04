//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_MODEL_RAILPART_H
#define MRW_MODEL_RAILPART_H

#include <regex>
#include <set>

#include <QDomElement>

#include <model/assemblypart.h>

namespace mrw::model
{
	class ModelRailway;

	class RailPart : public AssemblyPart
	{
		static const std::regex path_regex;

	protected:
		const bool              a_in_dir;
		std::set<RailPart *>    rail_forward;
		std::set<RailPart *>    rail_backward;

	public:
		explicit RailPart(
			ModelRailway     *    model_railway,
			const QDomElement  &  element);
		virtual ~RailPart() = default;

		inline bool aIsDir() const
		{
			return a_in_dir;
		}

		virtual bool contains(RailPart * rail, const bool dir) const;
		virtual bool valid() = 0;

	protected:
		RailPart * resolve(const char * attr) const;
		std::set<RailPart *> & advance(const bool dir);
		const std::set<RailPart *> & advance(const bool dir) const;
	};
}

#endif
