//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include "random.h"

using namespace mrw::util;

Random::Random() : random_number_generator(random_device())
{
}
