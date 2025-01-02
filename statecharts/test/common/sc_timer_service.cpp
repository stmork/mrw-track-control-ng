/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//
* */

#include "sc_timer_service.h"
#include <algorithm>

using namespace sc::timer;

void TimerService::setGenericTimer(const TimerTask::TaskData & data)
{
	// do nothing if there are no free slots
	if (next_free_task >= length)
	{
		return;
	}

	// Insert the task at the front
	size_t inserted_task_idx = next_free_task;
	TimerTask & inserted_task = tasks[inserted_task_idx];
	inserted_task.data = data;
	next_free_task = inserted_task.next_task_idx;
	inserted_task.next_task_idx = next_active_task;
	next_active_task = inserted_task_idx;
}


void TimerService::unsetGenericTimer(TimerTask::TimerTaskMatcher & matcher)
{
	size_t last_position = length;
	size_t next_position = next_active_task;

	while (next_position < length)
	{
		TimerTask & current_task = tasks[next_position];
		if (matcher.match(current_task))
		{
			current_task.reset();
			if (last_position < length)
			{
				tasks[last_position].next_task_idx = current_task.next_task_idx;
			}
			else
			{
				next_active_task = current_task.next_task_idx;
			}
			size_t current_position = next_position;
			next_position = current_task.next_task_idx;
			current_task.next_task_idx = next_free_task;
			next_free_task = current_position;
		}
		else
		{
			last_position = next_position;
			next_position = current_task.next_task_idx;
		}
	}
}

/*! Start the timing for a time event.
 */
void TimerService::setTimer(TimedInterface * statemachine, sc::eventid event,
	sc::time time_ms, bool isPeriodic)
{
	TimerTask::TaskData data(statemachine, event, time_ms, isPeriodic);
	setGenericTimer(data);
}

/*! Unset the given time event.
 */
void TimerService::unsetTimer(TimedInterface * statemachine, sc::eventid event)
{
	TimerTask::MatchTimeEvent matcher(statemachine, event);
	unsetGenericTimer(matcher);
}

/*! Set a timer for running cycles of the given statemachine.
 */
void TimerService::setRuncycleTimerFor(CycleBasedInterface * statemachine,  sc::time cycle_period)
{
	TimerTask::TaskData data(statemachine, cycle_period);
	setGenericTimer(data);
}

/*! Unset timers for running cycles of the given statemachine.
 */
void TimerService::unsetRuncycleTimerFor(CycleBasedInterface * statemachine)
{
	TimerTask::MatchRunCycleOf matcher(statemachine);
	unsetGenericTimer(matcher);
}

/*!
* This function must be called by the user. The elapsed time must be calculated every time,
* the function gets called.
*/
void TimerService::proceed(sc::time elapsed_ms)
{
	if (next_active_task >= length)
	{
		return;
	}

	sc::time time_to_proceed = std::min(time_till_next_task(), elapsed_ms);
	sc::time proceeded_time = 0;
	while (time_to_proceed > 0)
	{
		/* go through all active timers and update their time */
		size_t idx = next_active_task;
		while (idx < length)
		{
			TimerTask & current_task = this->tasks[idx];
			current_task.updateElapsedTimeMs(time_to_proceed);
			idx = current_task.next_task_idx;
		}

		bool task_fired;
		do
		{
			task_fired = false;
			size_t before_best = length;
			size_t best = next_active_task;
			sc::time best_remaining_time = tasks[best].data.get.time_ms - tasks[best].elapsed_time_ms;
			size_t last_task = best;
			size_t next_task = tasks[best].next_task_idx;
			while (next_task < length)
			{
				TimerTask & current_task = tasks[next_task];
				sc::time remaining_time = current_task.data.get.time_ms - current_task.elapsed_time_ms;
				if (remaining_time < best_remaining_time || (remaining_time == best_remaining_time && !(tasks[best] < current_task)))
				{
					best = next_task;
					before_best = last_task;
					best_remaining_time = remaining_time;
				}
				last_task = next_task;
				next_task = current_task.next_task_idx;
			}
			if (best_remaining_time <= 0)
			{
				TimerTask & best_task = tasks[best];
				TimerTask::TaskData data(best_task.data);
				if (best_task.isPeriodic())   // reset periodic time
				{
					best_task.elapsed_time_ms = 0;
				}
				else     // remove the task
				{
					best_task.reset();
					if (before_best < length)   // remove in list
					{
						tasks[before_best].next_task_idx = best_task.next_task_idx;
					}
					else     // remove head
					{
						next_active_task = best_task.next_task_idx;
					}
					best_task.next_task_idx = next_free_task;
					next_free_task = best;
				}

				// execute the task
				data.execute();
				task_fired = true;
			}
		}
		while (task_fired && next_active_task < length);

		proceeded_time += time_to_proceed;
		time_to_proceed = std::min(time_till_next_task(), elapsed_ms - proceeded_time);
	}
}

/*! Cancel timer service. Use this to end possible timing threads.
*/
void TimerService::cancel()
{
	for (size_t idx = 0; idx < length; ++idx)
	{
		tasks[idx].reset();
		tasks[idx].next_task_idx = idx + 1;
	}
	next_active_task = length;
	next_free_task = 0;
}

sc::time TimerService::time_till_next_task()
{
	if (next_active_task == length)
	{
		return 0;
	}

	sc::time time = tasks[next_active_task].data.get.time_ms - tasks[next_active_task].elapsed_time_ms;
	size_t task = tasks[next_active_task].next_task_idx;
	while (task < length)
	{
		TimerTask & current_task = tasks[task];
		sc::time remaining_time = current_task.data.get.time_ms - current_task.elapsed_time_ms;
		if (remaining_time < time)
		{
			time = remaining_time;
		}
		task = current_task.next_task_idx;
	}

	return time;
}

