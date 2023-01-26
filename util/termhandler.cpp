//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/termhandler.h>

using namespace mrw::util;

TermHandler::TermHandler(const int quit_signal) : TermHandler(
{
	quit_signal
})
{
}

TermHandler::TermHandler(const std::initializer_list<int> & quit_signals) :
	SignalHandler(quit_signals, []()
{
	QCoreApplication::quit();
})
{
}
