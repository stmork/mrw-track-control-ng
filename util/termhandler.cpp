//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include <QCoreApplication>

#include "termhandler.h"

using namespace mrw::util;

TermHandler::TermHandler(const int quit_signal) : TermHandler(
{
	quit_signal
})
{
}

TermHandler::TermHandler(const std::initializer_list<int> & quit_signals)
{
	sigset_t blocking_mask;
	struct sigaction sa;

	sigemptyset(&blocking_mask);
	for (auto sig : quit_signals)
	{
		sigaddset(&blocking_mask, sig);
	}
	sa.sa_handler = handler;
	sa.sa_mask    = blocking_mask;
	sa.sa_flags   = 0;

	for (auto sig : quit_signals)
	{
		sigaction(sig, &sa, nullptr);
	}
}

void TermHandler::handler(const int sig)
{
	Q_UNUSED(sig);

	QCoreApplication::quit();
}
