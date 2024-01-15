//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_SIGNALHANDLER_H
#define MRW_UTIL_SIGNALHANDLER_H

#include <functional>
#include <signal.h>
#include <unordered_map>

namespace mrw::util
{
	/**
	 * This class installs a posix signal callback handler. Whenever the signal
	 * occurs using the kill shell command the given callback is called.
	 */
	class SignalHandler
	{
	public:
		/** The type definition of the callback. */
		typedef std::function<void()> SignalCallback;

		/**
		 * This constructor registers a termination handler which reacts on the
		 * given list of signals.
		 *
		 * Example:
		 * @code
		 *  TermHandler   handler( { SIGTERM, SIGINT } );
		 *  Q_UNUSED(handler);
		 * @endcode
		 *
		 * @param signal_list The list of signals you want to handle.
		 * @param signal_callback The callback function you want to call on
		 * signalling.
		 *
		 * @see man 1 kill for further information.
		 */
		explicit SignalHandler(
			const std::initializer_list<int> & signal_list,
			SignalCallback                     signal_callback);

	private:
		static void handler(int sig);

		static std::unordered_map<int, SignalHandler *> map;

		SignalCallback callback;
	};
}

#endif
