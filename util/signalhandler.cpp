//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#include <util/signalhandler.h>

using namespace mrw::util;

std::unordered_map<int, SignalHandler *> SignalHandler::map;

SignalHandler::SignalHandler(
	const std::initializer_list<int> & signal_list,
	SignalCallback                     signal_callback)
{
	sigset_t         blocking_mask;
	struct sigaction sa;

	sigemptyset(&blocking_mask);
	for (const int sig : signal_list)
	{
		sigaddset(&blocking_mask, sig);
	}
	sa.sa_handler = handler;
	sa.sa_mask    = blocking_mask;
	sa.sa_flags   = 0;

	for (const int sig : signal_list)
	{
		if (sigaction(sig, &sa, nullptr) == 0)
		{
			map.emplace(sig, this);
		}
	}

	callback = signal_callback;
}

void SignalHandler::handler(int sig)
{
	auto it = map.find(sig);

	if (it != map.end())
	{
		it->second->callback();
	}
}
