//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#include <util/signalhandler.h>

#include <unistd.h>
#include <fcntl.h>

#include <QSocketNotifier>

using namespace mrw::util;

std::unordered_multimap<int, SignalHandler *> SignalHandler::handler_map;

SignalHandler::SignalHandler(const std::initializer_list<int> & signal_list)
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
			handler_map.emplace(sig, this);
		}
	}
}

void SignalHandler::handler(int sig)
{
	auto range = handler_map.equal_range(sig);

	for (auto it = range.first; it != range.second; ++it)
	{
		SignalHandler * sig_handler = it->second;

		sig_handler->callback(sig);
	}
}

/*****************************************************************************/

SigPipeHandler::SigPipeHandler(const std::initializer_list<int> & signal_list) :
	SignalHandler(signal_list)
{
	if (pipe(sig_pipe) < 0)
	{
		const char * error_text = strerror(errno);

		qCritical("Pipe init error: %s", error_text);
		throw std::runtime_error(error_text);
	}
	if (fcntl(sig_pipe[0], F_SETFL, O_NONBLOCK) < 0)
	{
		const char * error_text = strerror(errno);

		qCritical("Pipe config error: %s", error_text);
		throw std::runtime_error(error_text);
	}

	auto notifier = new QSocketNotifier(sig_pipe[0], QSocketNotifier::Read, this);

	QObject::connect(notifier, &QSocketNotifier::activated, [this]
	{
		int sig;
		if (read(sig_pipe[0], &sig, sizeof(sig)) == sizeof(sig))
		{
			emit signal(sig);
		}
		else
		{
			qCritical("Pipe read error: %s", strerror(errno));
		}
	});
}

void SigPipeHandler::callback(const int sig)
{
	if (write(sig_pipe[1], &sig, sizeof(sig)) != sizeof(sig))
	{
		qCritical("Pipe write error: %s", strerror(errno));
	}
}

/*****************************************************************************/

SigCallbackHandler::SigCallbackHandler(
	const std::initializer_list<int> & signal_list,
	SignalCallback                     signal_callback) :
	SigPipeHandler(signal_list)
{
	connect(
		this, &SigPipeHandler::signal,
		this, [signal_callback](int)
	{
		signal_callback();
	},
	Qt::QueuedConnection);
}
