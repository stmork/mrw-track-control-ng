//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_CTRL_RAILPARTINFO_H
#define MRW_CTRL_RAILPARTINFO_H

#include <model/railpart.h>
#include <model/section.h>
#include <model/region.h>

namespace mrw::ctrl
{
	class RailPartInfo
	{
	public:
		virtual mrw::model::RailPart * railPart() const = 0;

		mrw::model::Section * section() const;
		mrw::model::Region  * region() const;
	};
}

#endif