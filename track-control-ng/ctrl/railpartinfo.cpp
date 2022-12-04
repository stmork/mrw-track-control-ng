//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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
