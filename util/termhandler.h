//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_TERMHANDLER_H
#define MRW_UTIL_TERMHANDLER_H

#include <signal.h>
#include <initializer_list>

namespace mrw::util
{
	/**
	 * This class registeres a handler for terminating the application. The
	 * termination may be caused by pressing the CTRL+C keyboard sequence or
	 * following console command:
	 *
	 * @code
	 killall -SIGQUIT <application>
	 @endcode
		 *
		 * @note The application will terminate gracefully using
		 * the QCoreApplication::quit() method.
	 */
	class TermHandler
	{
	public:
		/**
		 * This constructor registers a termination handler which reacts on the
		 * given signal.
		 *
		 * Example:
		 * @code
		 *  SigTermHandler   handler(SIGTERM);
		 *  Q_UNUSED(handler);
		 * @endcode
		 *
		 * @param quit_signal The signal to use for
		 * termination, usual SIGTERM.
		 *
		 * @see man 1 kill for further information.
		 */
		explicit TermHandler(const int quit_signal);

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
		 * @param quit_signals The list of signals to use for
		 * termination, usual SIGTERM or SIGINT.
		 *
		 * @see man 1 kill for further information.
		 */
		explicit TermHandler(const std::initializer_list<int> & quit_signals);

	private:
		static void handler(const int sig);
	};
}

#endif

