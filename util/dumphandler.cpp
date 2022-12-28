//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#include "dumphandler.h"

using namespace mrw::util;

DumpHandler::Callback DumpHandler::callback = [] () {};

DumpHandler::DumpHandler(Callback dump_callback)
{
	sigset_t         blocking_mask;
	struct sigaction sa;

	callback = dump_callback;

	sigemptyset(&blocking_mask);
	sigaddset(&blocking_mask, SIGQUIT);

	sa.sa_handler = handler;
	sa.sa_mask    = blocking_mask;
	sa.sa_flags   = 0;

	sigaction(SIGQUIT, &sa, nullptr);
}

void DumpHandler::handler(const int sig)
{
	(void)sig;

	DumpHandler::callback();
}
