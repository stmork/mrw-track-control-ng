//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//

#pragma once

#ifndef MRW_STATECHART_TIMERSERVICE_H
#define MRW_STATECHART_TIMERSERVICE_H

#include <cassert>
#include <memory>

#include <QTimer>
#include <QMap>

#include <util/self.h>
#include <util/singleton.h>

#include <statecharts/common/sc_timer.h>

namespace mrw::statechart
{
	/**
	 * This class represents one single Yakindu SCT timer. It contains
	 * information about the causing statemachine, its duration and if the
	 * timer is periodic. Each instance keeps the next time point to wait
	 * until if the timer is active.
	 */
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

	/**
	 * This class simply implements an example statemachine for testing the
	 * C++ timer service. It is implemented as singleton.
	 *
	 * @see mrw::util::Singleton
	 */
	class TimerService :
		public QObject,
		public sc::timer::TimerServiceInterface,
		public mrw::util::Singleton<TimerService>
	{
		Q_OBJECT

	public:
		friend class mrw::util::Singleton<TimerService>;

		inline operator std::shared_ptr<TimerServiceInterface> & ()
		{
			Q_ASSERT(self.get() == this);

			return self;
		}

	private:
		TimerService();

		virtual void setTimer(
			std::shared_ptr<sc::timer::TimedInterface> statemachine,
			sc::eventid                                event,
			sc::time                                   time_ms,
			bool                                       is_periodic) override;
		virtual void unsetTimer(
			std::shared_ptr<sc::timer::TimedInterface> statemachine,
			sc::eventid                                event) override;

	private:
		/**
		 * This method lookups a timer for a specified statemachine. If the
		 * timer does not exists it will will be created automatically.
		 *
		 * @param machine The statechart which owns the timer.
		 * @param event The timer ID of that statechart.
		 * @return The SCTimer instance.
		 */
		SCTimer * getTimer(
			std::shared_ptr<sc::timer::TimedInterface> & statemachine,
			sc::eventid                                  event);

		/**
		 * This is the two dimensional key for finding a SCTimer instance.
		 */
		typedef std::pair<sc::timer::TimedInterface *, sc::eventid> TimerKey;

		/**
		 * This defines a map from a two dimensional key to a concrete
		 * SCTimer instance.
		 */
		typedef QMap<TimerKey, SCTimer *>                           TimerMap;

		/**
		 * The map from all statemachines and their IDs to all existing timers.
		 */
		TimerMap                                                    chart_map;

		/** This instance as shared pointer. */
		std::shared_ptr<TimerServiceInterface>                      self;
	};

	/**
	 * This template class encapsulates a QObject based statechart and
	 * a mrw::util::SelfPointer class for convenience purposes.
	 */
	template<class T> class QtStatechart :
		public T,
		public mrw::util::SelfPointer<T>
	{
	public:
		QtStatechart() : T(nullptr), mrw::util::SelfPointer<T>(this)
		{
		}
	};
}

#endif
