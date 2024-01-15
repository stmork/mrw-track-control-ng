//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/termhandler.h>

using namespace mrw::util;

TermHandler::TermHandler() : TermHandler( { SIGTERM, SIGINT } )
{
}

TermHandler::TermHandler(const std::initializer_list<int> & quit_signals) :
	SignalHandler(quit_signals, []()
{
	QCoreApplication::quit();
})
{
}
