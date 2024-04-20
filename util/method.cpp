//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <cinttypes>
#include "util/method.h"

using namespace mrw::util;

const QString Method::TIME_STAMP_FORMAT("yyyy/MM/dd h:mm:ss.zzz");

Method::Method(const char * method) : method_name(method)
{
	qDebug(">%s()", method_name);
	timer.start();
}

Method::~Method()
{
	qDebug("<%s() took %lld ms.", method_name, timer.elapsed());
}

void Method::pattern(const bool use_time_stamp) noexcept
{
	QString format;

	if (use_time_stamp)
	{
		format = "%{time " + TIME_STAMP_FORMAT + "} ";
	}

	format +=
		"%{if-debug}D%{endif}"
		"%{if-info}I%{endif}"
		"%{if-warning}W%{endif}"
		"%{if-critical}C%{endif}"
		"%{if-fatal}F%{endif} - %{message}";
	qSetMessagePattern(format);
}
