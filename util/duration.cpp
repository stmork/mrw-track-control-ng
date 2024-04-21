//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QDebug>

#include "util/duration.h"

using namespace mrw::util;

const QString Duration::TIME_STAMP_FORMAT("yyyy/MM/dd h:mm:ss.zzz");

void Duration::pattern(const bool use_time_stamp) noexcept
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

Duration::Duration(const char * naming) : name(naming)
{
	qDebug(">%s()", name);
	timer.start();
}

Duration::~Duration()
{
	qDebug("<%s() took %lld ms.", name, timer.elapsed());
}
