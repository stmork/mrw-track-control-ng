//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
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
	qDebug("<%s() took %lld ms.", method_name, timer.elapsed());
}

void Method::pattern() noexcept
{
	qSetMessagePattern(
		"%{time yyyy/MM/dd h:mm:ss.zzz} "
		"%{if-debug}D%{endif}"
		"%{if-info}I%{endif}"
		"%{if-warning}W%{endif}"
		"%{if-critical}C%{endif}"
		"%{if-fatal}F%{endif} - %{message}");
}
