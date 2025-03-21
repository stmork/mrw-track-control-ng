//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#include <util/signalhandler.h>

using namespace mrw::util;

std::unordered_map<int, SignalHandler *> SignalHandler::map;

SignalHandler::SignalHandler(
	const std::initializer_list<int> & signal_list,
	SignalCallback                     signal_callback)
{
	struct sigaction sa {};

	sigemptyset(&sa.sa_mask);
	for (const int sig : signal_list)
	{
		sigaddset(&sa.sa_mask, sig);
	}
	sa.sa_handler = handler;
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
