//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_DUMPHANDLER_H
#define MRW_UTIL_DUMPHANDLER_H

#include <signal.h>
#include <functional>

namespace mrw::util
{
	/**
	 * This class registeres a callback which can be used to dump information
	 * during runtime execution. The call occurs by using the following console
	 * command or pressing CTRL + \\:
	 * @code
	 killall -SIGQUIT <application>
	 @endcode
	 *
	 * @note The application will <em>not</em> terminate and
	 * continues execution.
	 */
	class DumpHandler
	{
	public:
		/** The type definition of the callback. */
		typedef std::function<void()> Callback;

		/**
		 * This constructor registers a callback to react on the POSIX SIGQUIT
		 * signal like the stack dump of a Java process. Pressing CTRL + \\
		 * in a terminal causes sending this signal to a running process. This
		 * can be useful to dump some information inside the callback.
		 *
		 * @see man 1 kill for further information.
		 * @param dump_callback The method to call if a SIGQUIT occurs.
		 *
		 * Example:
		 * @code
		 * DumpHandler   handler([]() {  printf("Hello\n");  });
		 * @endcode
		 */
		explicit DumpHandler(Callback dump_callback);

	private:
		static void handler(int sig);

		static Callback callback;
	};
}

#endif
