/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//
* */

#ifndef SC_TIMER_H_
#define SC_TIMER_H_

#include "sc_types.h"
#include <cstddef>

namespace sc
{
	namespace timer
	{

		class TimedInterface;
		class TimerServiceInterface;

		/*! \file
		Interface for state machines which use timed event triggers.
		*/
		class TimedInterface
		{
		public:

			virtual ~TimedInterface() = 0;

			/*!
			Set the timer service for the state machine. It must be set
			externally on a timed state machine before a run cycle can be executed.
			*/
			virtual void setTimerService(sc::timer::TimerServiceInterface * timerService) = 0;

			/*!
			Return the currently used timer service.
			*/
			virtual sc::timer::TimerServiceInterface * getTimerService() = 0;

			/*!
			Callback method if a time event occurred.
			*/
			virtual void raiseTimeEvent(sc::eventid event) = 0;

			/*!
			Retrieve the number of time events that can be active at once in this state machine.
			*/
			virtual sc::integer getNumberOfParallelTimeEvents() = 0;

		};

		inline TimedInterface::~TimedInterface() {}


		/*! \file
		Timer service interface.
		*/
		class TimerServiceInterface
		{
		public:

			virtual ~TimerServiceInterface() = 0;

			/*!
			Starts the timing for a time event.
			*/
			virtual void setTimer(TimedInterface * statemachine, sc::eventid event, sc::time time_ms, bool isPeriodic) = 0;

			/*!
			Unsets the given time event.
			*/
			virtual void unsetTimer(TimedInterface * statemachine, sc::eventid event) = 0;
		};

		inline TimerServiceInterface::~TimerServiceInterface() {}

	} /* namespace sc::timer */
} /* namespace sc */

#endif /* SC_TIMER_H_ */
