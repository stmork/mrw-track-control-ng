//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#pragma once

#ifndef MRW_UTIL_CLOCKSERVICE_H
#define MRW_UTIL_CLOCKSERVICE_H

#include <QObject>
#include <QTimer>

#include <util/singleton.h>

namespace mrw::util
{
	class ClockService : public QObject, public Singleton<ClockService>
	{
		friend class Singleton<ClockService>;

		Q_OBJECT

	private:
		QTimer timer_1hz;
		QTimer timer_2hz;
		QTimer timer_4hz;
		QTimer timer_8hz;

		ClockService();
	signals:
		void Hz1();
		void Hz2();
		void Hz4();
		void Hz8();
	};
}

#endif
