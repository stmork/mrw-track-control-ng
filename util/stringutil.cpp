//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "util/stringutil.h"

using namespace mrw::util;

const QString String::RED_ON        = "\e[31m";
const QString String::GREEN_ON      = "\e[32m";
const QString String::BLUE_ON       = "\e[34m";
const QString String::MAGENTA_ON    = "\e[35m";
const QString String::CYAN_ON       = "\e[36m";
const QString String::WHITE_ON      = "\e[37m";

const QString String::BOLD_ON       = "\e[1m";
const QString String::ITALIC_ON     = "\e[3m";
const QString String::UNDERLINE_ON  = "\e[4m";
const QString String::ALL_OFF       = "\e[0m";

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
