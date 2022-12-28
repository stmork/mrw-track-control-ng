//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

#pragma once

#ifndef MRW_STATECHART_TIMERSERVICE_H
#define MRW_STATECHART_TIMERSERVICE_H

#include <QTimer>
#include <QMap>

#include <util/singleton.h>

#include <statecharts/common/sc_timer.h>

namespace mrw::statechart
{
	class SCTimer : public QTimer
	{
		Q_OBJECT

	public:
		explicit SCTimer(
			QObject          *          parent,
			sc::timer::TimedInterface * statemachine,
			const sc::eventid           id);
		SCTimer() = delete;

	private slots:
		void trigger();

	private:
		sc::timer::TimedInterface * machine;
		sc::eventid                 event_id;
	};

	class TimerService :
		public QObject,
		public sc::timer::TimerServiceInterface,
		public mrw::util::Singleton<TimerService>
	{
		Q_OBJECT

	public:
		friend class mrw::util::Singleton<TimerService>;

	private:
		TimerService();

		virtual void setTimer(
			sc::timer::TimedInterface * statemachine,
			sc::eventid                 event,
			sc::integer                 time_ms,
			bool                        is_periodic) override;
		virtual void unsetTimer(
			sc::timer::TimedInterface * statemachine,
			sc::eventid                 event) override;

		SCTimer * getTimer(sc::timer::TimedInterface * machine, sc::eventid event);

		typedef std::pair<sc::timer::TimedInterface *, sc::eventid> TimerKey;
		typedef QMap<TimerKey, SCTimer *>                           TimerMap;

		TimerMap chart_map;
	};
}

#endif
