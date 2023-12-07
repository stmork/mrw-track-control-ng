//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "util/stringutil.h"

using namespace mrw::util;

const QString String::RED_ON        = "\033[31m";
const QString String::GREEN_ON      = "\033[32m";
const QString String::BLUE_ON       = "\033[34m";
const QString String::MAGENTA_ON    = "\033[35m";
const QString String::CYAN_ON       = "\033[36m";
const QString String::WHITE_ON      = "\033[37m";

const QString String::BOLD_ON       = "\033[1m";
const QString String::ITALIC_ON     = "\033[3m";
const QString String::UNDERLINE_ON  = "\033[4m";
const QString String::ALL_OFF       = "\033[0m";

String::operator QString() const
{
	return toString();
}

QString String::format(const QString & code, const QString & input)
{
	return code + input + ALL_OFF;
}

QString String::bold(const QString & input)
{
	return format(BOLD_ON, input);
}

QString String::red(const QString & input)
{
	return format(RED_ON, input);
}

std::ostream & operator<<(std::ostream & os, const String & instance)
{
	os << instance.toString().toStdString();

	return os;
}

QDebug operator<<(QDebug debug, const String & instance)
{
	QDebugStateSaver saver(debug);

	debug.nospace().noquote() << instance.toString();

	return debug;
}

QString operator+(const QString & left, const String & right)
{
	return left + right.toString();
}
