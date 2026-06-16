//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_SIGNALHANDLER_H
#define MRW_UTIL_SIGNALHANDLER_H

#include <functional>
#include <signal.h>
#include <unordered_map>

#include <QObject>

namespace mrw::util
{
	/**
	 * This class installs a posix signal callback handler. Whenever the signal
	 * occurs using the kill shell command the given callback is called.
	 */
	class SignalHandler : public QObject
	{
		Q_OBJECT

	public:
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
		 * signalling.
		 *
		 * @see man 1 kill for further information.
		 */
		explicit SignalHandler(const std::initializer_list<int> & signal_list);

	protected:
		/**
		 * This callback method is called whenever a registered signal occurs.
		 *
		 * @note This method is called in the context of a POSIX signal handler
		 * and therefore only async-signal-safe functions are allowed to be
		 * used.
		 *
		 * @param sig The signal number which occurred.
		 */
		virtual void callback(const int sig) = 0;

	private:
		static void handler(int sig);

		static std::unordered_multimap<int, SignalHandler *> handler_map;
	};

	/*************************************************************************/

	/**
	 * This class implements a signal handler which is thread safe against any
	 * event loop using a POSIX pipe mechanism. The implemented callback writes
	 * into a pipe and the reading end is monitored by a QSocketNotifier which
	 * emits the Qt signal to which you can connect to. This mechanism is a
	 * safe way to handle POSIX signals in a Qt application.
	 *
	 * @note You can directly use this class with connecting to the signal()
	 * signal.
	 */
	class SigPipeHandler : public SignalHandler
	{
		Q_OBJECT

	public:
		SigPipeHandler(const std::initializer_list<int> & signal_list);

	protected:
		int sig_pipe[2] { -1, -1};

	signals:
		void signal(const int sig);

	private:
		void callback(const int sig) override;
	};

	/*************************************************************************/

	/**
	 * This class implements a signal handler which calls a given callback
	 * without the need to connect this class to a Qt slot. Under the hood
	 * it uses the SigPipeHandler's signal() signal to call the given callback.
	 */
	class SigCallbackHandler : public SigPipeHandler
	{
		Q_OBJECT

	public:
		/** The type definition of the callback. */
		typedef std::function<void()> SignalCallback;

		explicit SigCallbackHandler(
			const std::initializer_list<int> & signal_list,
			SignalCallback                     signal_callback);
	};
}

#endif
