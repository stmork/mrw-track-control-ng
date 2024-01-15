//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include "railpartinfo.h"

using namespace mrw::model;
using namespace mrw::ctrl;

Section * RailPartInfo::section() const
{
	return railPart()->section();
}

Region * RailPartInfo::region() const
{
	return section()->region();
}
