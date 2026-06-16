//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <QCoreApplication>

#include <util/termhandler.h>

using namespace mrw::util;

TermHandler::TermHandler() : TermHandler( { SIGTERM, SIGINT } )
{
}

TermHandler::TermHandler(const std::initializer_list<int> & quit_signals) :
	SigPipeHandler(quit_signals)
{
	connect(
		this, &SigPipeHandler::signal,
		QCoreApplication::instance(), &QCoreApplication::quit,
		Qt::QueuedConnection);
}
