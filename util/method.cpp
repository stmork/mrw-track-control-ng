//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <cinttypes>
#include "util/method.h"

using namespace mrw::util;

Method::Method(const char * method) : method_name(method)
{
	qDebug(">%s()", method_name);
	timer.start();
}

Method::~Method()
{
	unsigned elapsed = timer.elapsed();

	qDebug("<%s() took %u ms.", method_name, elapsed);
}
