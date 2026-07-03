//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_CLOCKSERVICE_H
#define MRW_UTIL_CLOCKSERVICE_H

#include <QTimer>

#include <util/singleton.h>

namespace mrw::util
{
	/**
	 * This singleton class provides a regular clock service at the clocks of
	 * 1 Hz, 2 Hz, 4 Hz and 8 Hz. You can simply connect to the appropriate
	 * signals in the usual Qt manner. To provide a synchron clock you should
	 * connect using the @c Qt::DirectConnection type.
	 */
	class ClockService : public QObject, public Singleton<ClockService>
	{
		friend class Singleton<ClockService>;

		Q_OBJECT

		unsigned tick_counter = 0;

	private:
		QTimer timer;

		ClockService();

	signals:
		/**
		 * The one per second clock.
		 */
		void Hz1(const unsigned counter);

		/**
		 * The two per second clock.
		 */
		void Hz2(const unsigned counter);

		/**
		 * The four per second clock.
		 */
		void Hz4(const unsigned counter);

		/**
		 * The eight per second clock.
		 */
		void Hz8(const unsigned counter);

	private slots:
		void tick();
	};
}

#endif
