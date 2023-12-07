/*
 * Timer Service Implementation for SCTUnit
 */

#include "sc_runner_timed.h"

/**
 * Implementation of a timer service that uses _virtual_ time to raise time events.
 * It is solely meant for use with sctunit.
 */

TimedSctUnitRunner::TimedSctUnitRunner(
	sc::StatemachineInterface * statemachine_,
	bool event_driven_,
	sc::integer cycle_period_
) :
	statemachine(statemachine_),
	event_driven(event_driven_),
	cycle_period(event_driven_ ? -1 : cycle_period_),
	current_time_ms(0),
	timer_queue()
{
	if (!event_driven)
	{
		SctTimer runCycle(
			0,
			cycle_period,
			true,
			0,
			-1,
			true
		);
		insert_timer(runCycle);
	}
}

void TimedSctUnitRunner::proceed_time(sc::integer time_ms)
{
	sc::integer stop_time_ms = current_time_ms + time_ms;
	bool processed_timer = false;

	do
	{

		// first assume we won't process a timer
		processed_timer = false;

		// and then check if there is a timer to process
		if ( ! timer_queue.empty())
		{
			SctTimer next = timer_queue.front();

			if (next.abs_time_ms <= stop_time_ms)
			{
				timer_queue.pop_front();
				current_time_ms = next.abs_time_ms;

				/* Repeat and reinsert timer? */
				if (next.periodic)
				{
					insert_timer(next);
				}
				next.tsi->raiseTimeEvent(next.pt_evid);
				processed_timer = true;
			}
		}

	}
	while ( processed_timer );

	// As a postcondition the current time is the time after proceeding the specified period.
	current_time_ms = stop_time_ms;
}

void TimedSctUnitRunner::proceed_cycles(sc::integer cycles)
{
	sc::integer elapsed_cycles = 0;

	while (elapsed_cycles < cycles)
	{
		if (timer_queue.empty())
		{
			return;
		}
		SctTimer next = timer_queue.front();

		timer_queue.pop_front();

		current_time_ms = next.abs_time_ms;

		std::list<SctTimer>::iterator i_timer;

		/* Repeat and reinsert timer? */
		if (next.periodic)
		{
			insert_timer(next);
		}
		next.tsi->raiseTimeEvent(next.pt_evid);
	}
}
void TimedSctUnitRunner::setTimer(sc::timer::TimedInterface * statemachine_, sc::eventid event, sc::integer time_ms, bool isPeriodic)
{
	SctTimer timer(statemachine_, time_ms, isPeriodic, event, 0, false);
	insert_timer(timer);
}

void TimedSctUnitRunner::unsetTimer(sc::timer::TimedInterface * statemachine_, sc::eventid event)
{
	std::list<SctTimer>::iterator i_timer = timer_queue.begin();
	while (i_timer != timer_queue.end() && i_timer->tsi == statemachine_)
	{
		if (i_timer->pt_evid == event)
		{
			timer_queue.erase(i_timer);
			return;
		}
		++i_timer;
	}
}

void TimedSctUnitRunner::cancel()
{
	std::list<SctTimer>::iterator i_timer = timer_queue.begin();
	while (i_timer != timer_queue.end())
	{
		i_timer = timer_queue.erase(i_timer);
	}
}
void TimedSctUnitRunner::insert_timer(SctTimer timer)
{
	timer.abs_time_ms = current_time_ms + timer.rel_time_ms;

	std::list<SctTimer>::iterator i_timer;

	/* Either the list is empty, so we put the new timer in directly... */
	if (timer_queue.empty())
	{
		timer_queue.push_front(timer);
		return;
	}


	/* Or we put it before some other timer that needs to be raised after this one */
	for (i_timer = timer_queue.begin(); i_timer != timer_queue.end(); ++i_timer)
	{
		if (timer.compare(&(*i_timer)) < 0)
		{
			timer_queue.insert(i_timer, timer);
			return;
		}
	}

	/* Or it gets put into the back of the list. */
	timer_queue.push_back(timer);
}

TimedSctUnitRunner::SctTimer::SctTimer(
	sc::timer::TimedInterface * tsi_,
	sc::integer time_ms,
	bool timer_periodic,
	sc::eventid evid,
	sc::integer timer_priority,
	bool timer_is_runcycle
) :
	tsi(tsi_),
	rel_time_ms(time_ms),
	abs_time_ms(0),
	periodic(timer_periodic),
	pt_evid(evid),
	priority(timer_priority),
	is_runcycle(timer_is_runcycle)
{}

sc::integer TimedSctUnitRunner::SctTimer::compare(SctTimer * other)
{
	sc::integer result = abs_time_ms - other->abs_time_ms;
	if (result != 0)
	{
		return result;
	}
	else
	{
		// bigger priority needs to be raised first
		result = other->priority - priority;
		return result;
	}
}
