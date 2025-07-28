//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_APPSUPPORT_H
#define MRW_UTIL_APPSUPPORT_H

#include <QString>

#include <util/singleton.h>

namespace mrw::util
{
	/**
	 * This class returns some values that are useful for the application.
	 */
	class AppSupport : public Singleton<AppSupport>
	{
		friend class Singleton<AppSupport>;

		AppSupport();

	public:
		static AppSupport & instance() noexcept;

		/**
		 * This method returns a unified form of the local hostname. This
		 * hostname contains only the first part of the hostname without
		 * any domain information. The hostname is always lower case.
		 *
		 * @return The hostname of the local system.
		 */
		QString hostname() const noexcept;

		/**
		 * This method returns the blanktime specified in the kernel
		 * boot parameter @c consoleblank. The value is clamped to return
		 * any valid blanktime greater than zero.
		 *
		 * @return The blank time configured for the kernel.
		 */
		unsigned blanktime() const noexcept;
	};
}

#endif
