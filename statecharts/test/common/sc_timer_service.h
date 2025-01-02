/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//
* */
#ifndef SC_TIMER_SERVICE_H_
#define SC_TIMER_SERVICE_H_

#include <cstddef>
#include "sc_timer.h"
#include "sc_types.h"
#include "sc_cyclebased.h"

namespace sc
{
	namespace timer
	{

		class TimerTask
		{
		private:
			struct TimeBased
			{
				TimedInterface * handle;
				sc::eventid pt_evid;
				bool periodic;
				TimeBased() : handle(nullptr), pt_evid(0), periodic(false) {}
			};
			struct NonEmpty
			{
				sc::time time_ms;
				TimeBased time_event;
				CycleBasedInterface * run_cycle_handle;
				NonEmpty() : time_ms(0), time_event(), run_cycle_handle(nullptr) {}
			};
		public:
			struct TaskData
			{
				enum TaskType
				{
					EMPTY_TASK,
					TIME_EVENT_TASK,
					RUNCYCLE_TASK
				};
				TaskType type;
				NonEmpty get;
				TaskData() : type(EMPTY_TASK) {}
				TaskData(TimedInterface * time_event_handle, sc::eventid pt_evid, sc::time time_ms, bool periodic) :
					type(TIME_EVENT_TASK)
				{
					get.time_ms = time_ms;
					get.time_event.handle = time_event_handle;
					get.time_event.pt_evid = pt_evid;
					get.time_event.periodic = periodic;
				}
				TaskData(CycleBasedInterface * run_cycle_handle, sc::time time_ms) :
					type(RUNCYCLE_TASK)
				{
					get.time_ms = time_ms;
					get.run_cycle_handle = run_cycle_handle;
				}

				void reset()
				{
					switch (type)
					{
					case TIME_EVENT_TASK:
						get.time_ms = 0;
						get.time_event.handle = nullptr;
						get.time_event.pt_evid = 0;
						get.time_event.periodic = false;
						break;
					case RUNCYCLE_TASK:
						get.time_ms = 0;
						get.run_cycle_handle = nullptr;
						break;
					default:
						return;
					}
					type = EMPTY_TASK;
				}

				virtual void execute()
				{
					switch (type)
					{
					case TIME_EVENT_TASK:
						{
							// fire the event
							TimedInterface * time_event_handle = get.time_event.handle;
							if (time_event_handle != nullptr)
							{
								time_event_handle->raiseTimeEvent(get.time_event.pt_evid);
							}
							return;
						}
					case RUNCYCLE_TASK:
						{
							CycleBasedInterface * run_cycle_handle = get.run_cycle_handle;
							if (run_cycle_handle != nullptr)
							{
								run_cycle_handle->runCycle();
							}
							return;
						}
					default:
						return;
					}
				}
				virtual ~TaskData() {};
			};

			TaskData data;
			sc::time elapsed_time_ms;
			size_t next_task_idx;

			TimerTask() : //
				data(), //
				elapsed_time_ms(0), //
				next_task_idx(0)
			{
			}

			TimerTask(const TaskData & data_) : //
				data(data_), //
				elapsed_time_ms(0), //
				next_task_idx(0)
			{
			}

			virtual ~TimerTask()
			{
			}

			void updateElapsedTimeMs(sc::time elapsed_time_ms_ = 0)
			{
				elapsed_time_ms += elapsed_time_ms_;
			}

			virtual bool isPeriodic() const
			{
				switch (data.type)
				{
				case TaskData::TIME_EVENT_TASK:
					return data.get.time_event.periodic;
				case TaskData::RUNCYCLE_TASK:
					return true;
				default:
					return false;
				}
			}

			virtual bool isRunCycleEvent() const
			{
				return data.type == TaskData::RUNCYCLE_TASK;
			}

			virtual void reset()
			{
				data.reset();
				elapsed_time_ms = 0;
			}

			/*
			Compare tasks based on their execution order.
			Return true if this task is always to be executed strictly before the other task if both are scheduled to run at the same time.
			Default behavior:
			- This task is to be scheduled strictly before the other task if its isRunCycleEvent() method does not return true and the other task's isRunCycleEvent() method returns true.
			*/
			virtual bool operator<(const TimerTask & other) const
			{
				return (!isRunCycleEvent() && other.isRunCycleEvent());
			}

			class TimerTaskMatcher
			{
			public:
				virtual ~TimerTaskMatcher() {}
				virtual bool match(const TimerTask & other) = 0;
			};
			class MatchTimeEvent : public TimerTaskMatcher
			{
			private:
				TimedInterface * time_event_handle;
				sc::eventid pt_evid;
			public:
				MatchTimeEvent(TimedInterface * time_event_handle_, sc::eventid pt_evid_): time_event_handle(time_event_handle_), pt_evid(pt_evid_) {}

				virtual bool match(const TimerTask & other)  override
				{
					return other.data.type == TaskData::TIME_EVENT_TASK && time_event_handle == other.data.get.time_event.handle && pt_evid == other.data.get.time_event.pt_evid;
				}

				virtual ~MatchTimeEvent() {}
			};

			class MatchRunCycleOf : public TimerTaskMatcher
			{
			private:
				CycleBasedInterface * run_cycle_handle;
			public:
				MatchRunCycleOf(CycleBasedInterface * run_cycle_handle_): run_cycle_handle(run_cycle_handle_) {}

				virtual bool match(const TimerTask & other)  override
				{
					return other.data.type == TaskData::RUNCYCLE_TASK && run_cycle_handle == other.data.get.run_cycle_handle;
				}

				virtual ~MatchRunCycleOf() {}
			};
		};

		class TimerService: public TimerServiceInterface
		{
		private:
			size_t length;
			TimerTask * tasks;
			size_t next_active_task;
			size_t next_free_task;
		protected:
			void setGenericTimer(const TimerTask::TaskData & data);
			void unsetGenericTimer(TimerTask::TimerTaskMatcher & matcher);
		public:

			TimerService(TimerTask * tasks_, size_t length_) :
				length(length_), //
				tasks(tasks_), //
				next_active_task(length_), //
				next_free_task(0) //
			{
				for (size_t i = 0; i < length; ++i)
				{
					tasks[i].next_task_idx = i + 1;
				}
			}

			virtual ~TimerService() {}

			/*! Start the timing for a time event.
			 */
			virtual void setTimer(TimedInterface * statemachine_, sc::eventid event,
				sc::time time_ms, bool isPeriodic) override;

			/*! Unset the given time event.
			 */
			virtual void unsetTimer(TimedInterface * statemachine_, sc::eventid event) override;

			/*! Set a timer for running cycles of the given statemachine.
			 */
			virtual void setRuncycleTimerFor(CycleBasedInterface * statemachine, sc::time cycle_period);

			/*! Unset timers for running cycles of the given statemachine.
			 */
			virtual void unsetRuncycleTimerFor(CycleBasedInterface * statemachine);

			/*!
			 * This function must be called by the user. The elapsed time must be calculated every time,
			 * the function gets called.
			 */
			virtual void proceed(sc::time elapsed_ms);

			/*! Cancel timer service. Use this to end possible timing threads and free
			 memory resources.
			 */
			virtual void cancel();

			/*! Obtain the time (in ms) required to proceed to the next task.
			 */
			sc::time time_till_next_task();
		};

	} /* namespace sc::timer */
} /* namespace sc */

#endif /* SC_TIMER_SERVICE_H_ */

