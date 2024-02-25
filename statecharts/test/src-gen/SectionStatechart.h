/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//
* */

#ifndef SECTIONSTATECHART_H_
#define SECTIONSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the SectionStatechart state machine.
		*/
		class SectionStatechart;
	}
}


#include <deque>
#include "../common/sc_types.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_eventdriven.h"
#include "../common/sc_timer.h"
#include <string.h>

/*! \file
Header of the state machine 'SectionStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SectionStatechart : public sc::timer::TimedInterface, public sc::EventDrivenInterface
		{
		public:
			SectionStatechart() noexcept;

			virtual ~SectionStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Init,
				main_region_Init_Init_Process_Requesting,
				main_region_Init_Init_Process_Requesting_relais_Relay,
				main_region_Init_Init_Process_Requesting_relais_Wait,
				main_region_Init_Init_Process_Requesting_state_Occupation,
				main_region_Init_Init_Process_Requesting_state_Wait,
				main_region_Operating,
				main_region_Operating_Processing_Unlocked,
				main_region_Operating_Processing_Locked,
				main_region_Operating_Processing_Locked_Route_active_Enabled,
				main_region_Operating_Processing_Locked_Route_active_Passed,
				main_region_Operating_Processing_Locked_Route_active_Waiting,
				main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left,
				main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling,
				main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling,
				main_region_Operating_Processing_Locked_Route_active_Disabled,
				main_region_Operating_Processing_Locked_Route_active_Wait_for_Unlock,
				main_region_Operating_Processing_Locked_Occupation_Free,
				main_region_Operating_Processing_Locked_Occupation_Occupied,
				main_region_Operating_Processing_Locked_Occupation__final_,
				main_region_Operating_Processing_Locked_Occupation_Next_Reached,
				main_region_Operating_Processing_Pending,
				main_region_Operating_Processing_Pending_Relais_processing_Enabling,
				main_region_Operating_Processing_Pending_Relais_processing_Disabling,
				main_region_Failed,
				main_region_Wait_for_Start
			};

			/*! The number of states. */
			static constexpr const sc::integer numStates {26};
			static constexpr const sc::integer scvi_main_region_Init {0};
			static constexpr const sc::integer scvi_main_region_Init_Init_Process_Requesting {0};
			static constexpr const sc::integer scvi_main_region_Init_Init_Process_Requesting_relais_Relay {0};
			static constexpr const sc::integer scvi_main_region_Init_Init_Process_Requesting_relais_Wait {0};
			static constexpr const sc::integer scvi_main_region_Init_Init_Process_Requesting_state_Occupation {1};
			static constexpr const sc::integer scvi_main_region_Init_Init_Process_Requesting_state_Wait {1};
			static constexpr const sc::integer scvi_main_region_Operating {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Unlocked {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Route_active_Enabled {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Route_active_Passed {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Route_active_Waiting {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Route_active_Disabled {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Route_active_Wait_for_Unlock {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Occupation_Free {1};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Occupation_Occupied {1};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Occupation__final_ {1};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked_Occupation_Next_Reached {1};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Relais_processing_Enabling {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Relais_processing_Disabling {0};
			static constexpr const sc::integer scvi_main_region_Failed {0};
			static constexpr const sc::integer scvi_main_region_Wait_for_Start {0};

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				enable,
				disable,
				clear,
				start,
				relaisResponse,
				stateResponse,
				failed,
				next,
				unlock,
				_te0_main_region_Init_,
				_te1_main_region_Operating_Processing_Locked_Route_active_Waiting_,
				_te2_main_region_Operating_Processing_Pending_,
				Internal_local_leave
			};

			class EventInstance
			{
			public:
				explicit  EventInstance(Event id) noexcept : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			template <typename T>
			class EventInstanceWithValue : public EventInstance
			{
			public:
				explicit  EventInstanceWithValue(Event id, T val) noexcept :
					EventInstance(id),
					value(val)
				{}
				virtual ~EventInstanceWithValue() = default;
				const T value;
			};
			/*! Raises the in event 'enable' of default interface scope. */
			void raiseEnable(bool enable_);
			/*! Raises the in event 'disable' of default interface scope. */
			void raiseDisable();
			/*! Raises the in event 'clear' of default interface scope. */
			void raiseClear();
			/*! Raises the in event 'start' of default interface scope. */
			void raiseStart();
			/*! Raises the in event 'relaisResponse' of default interface scope. */
			void raiseRelaisResponse();
			/*! Raises the in event 'stateResponse' of default interface scope. */
			void raiseStateResponse(bool stateResponse_);
			/*! Raises the in event 'failed' of default interface scope. */
			void raiseFailed();
			/*! Raises the in event 'next' of default interface scope. */
			void raiseNext();
			/*! Raises the in event 'unlock' of default interface scope. */
			void raiseUnlock();
			/*! Check if event 'started' of default interface scope is raised. */
			bool isRaisedStarted() noexcept;
			/*! Check if event 'stop' of default interface scope is raised. */
			bool isRaisedStop() noexcept;
			/*! Check if event 'entered' of default interface scope is raised. */
			bool isRaisedEntered() noexcept;
			/*! Check if event 'leave' of default interface scope is raised. */
			bool isRaisedLeave() noexcept;
			/*! Check if event 'leaving' of default interface scope is raised. */
			bool isRaisedLeaving() noexcept;
			/*! Check if event 'left' of default interface scope is raised. */
			bool isRaisedLeft() noexcept;
			/*! Check if event 'unregister' of default interface scope is raised. */
			bool isRaisedUnregister() noexcept;


			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			static sc::integer getTimeout()  noexcept;
			/*! Gets the value of the variable 'auto_off' that is defined in the default interface scope. */
			bool getAuto_off() const noexcept;
			/*! Sets the value of the variable 'auto_off' that is defined in the default interface scope. */
			void setAuto_off(bool auto_off) noexcept;
			/*! Gets the value of the variable 'auto_unlock' that is defined in the default interface scope. */
			bool getAuto_unlock() const noexcept;
			/*! Sets the value of the variable 'auto_unlock' that is defined in the default interface scope. */
			void setAuto_unlock(bool auto_unlock) noexcept;
			/*! Gets the value of the variable 'occupied' that is defined in the default interface scope. */
			bool getOccupied() const noexcept;
			/*! Sets the value of the variable 'occupied' that is defined in the default interface scope. */
			void setOccupied(bool occupied) noexcept;
			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void inc() = 0;

				virtual void dec() = 0;

				virtual void off() = 0;

				virtual void on() = 0;

				virtual void request() = 0;

				virtual void passed() = 0;

				virtual void free() = 0;

				virtual void leftBefore() = 0;

				virtual void fail() = 0;

				virtual void pending() = 0;

				virtual void lock(bool do_it) = 0;


			};

			/*! Set the working instance of the operation callback interface 'OperationCallback'. */
			void setOperationCallback(OperationCallback * operationCallback) noexcept;

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent() override;
			/*
			 * Functions inherited from StatemachineInterface
			 */
			void enter() override;

			void exit() override;

			/*!
			 * Checks if the state machine is active (until 2.4.1 this method was used for states).
			 * A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
			 */
			bool isActive() const noexcept override;


			/*!
			* Checks if all active states are final.
			* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
			*/
			bool isFinal() const noexcept override;

			/*!
			 * Checks if member of the state machine must be set. For example an operation callback.
			 */
			bool check() const noexcept;

			/*
			 * Functions inherited from TimedStatemachineInterface
			 */
			void setTimerService(sc::timer::TimerServiceInterface * timerService_) noexcept override;

			sc::timer::TimerServiceInterface * getTimerService() noexcept override;

			void raiseTimeEvent(sc::eventid event) override;

			sc::integer getNumberOfParallelTimeEvents() noexcept override;



			/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
			bool isStateActive(State state) const noexcept;

			//! number of time events used by the state machine.
			static const sc::integer timeEventsCount {3};

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount {1};


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			std::deque<EventInstance *> internalEventQueue;

			EventInstance * getNextEvent() noexcept;

			bool dispatchEvent(EventInstance * event) noexcept;



		private:
			SectionStatechart(const SectionStatechart & rhs);
			SectionStatechart & operator=(const SectionStatechart &);

			static constexpr const sc::integer timeout {500};
			bool auto_off {true};
			bool auto_unlock {true};
			bool occupied {false};





			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates {2};

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];



			OperationCallback * ifaceOperationCallback;

			bool isExecuting {false};
			sc::integer stateConfVectorPosition {0};
			bool stateConfVectorChanged {false};



			// prototypes of all internal functions

			void enact_main_region_Init();
			void enact_main_region_Init_Init_Process_Requesting();
			void enact_main_region_Init_Init_Process_Requesting_relais_Relay();
			void enact_main_region_Init_Init_Process_Requesting_state_Occupation();
			void enact_main_region_Operating_Processing_Unlocked();
			void enact_main_region_Operating_Processing_Locked();
			void enact_main_region_Operating_Processing_Locked_Route_active_Passed();
			void enact_main_region_Operating_Processing_Locked_Route_active_Waiting();
			void enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left();
			void enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling();
			void enact_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling();
			void enact_main_region_Operating_Processing_Locked_Occupation_Occupied();
			void enact_main_region_Operating_Processing_Locked_Occupation_Next_Reached();
			void enact_main_region_Operating_Processing_Pending();
			void enact_main_region_Operating_Processing_Pending_Relais_processing_Enabling();
			void enact_main_region_Operating_Processing_Pending_Relais_processing_Disabling();
			void enact_main_region_Failed();
			void exact_main_region_Init();
			void exact_main_region_Operating_Processing_Locked();
			void exact_main_region_Operating_Processing_Locked_Route_active_Waiting();
			void exact_main_region_Operating_Processing_Pending();
			void enseq_main_region_Init_default();
			void enseq_main_region_Init_Init_Process_Requesting_relais_Relay_default();
			void enseq_main_region_Init_Init_Process_Requesting_relais_Wait_default();
			void enseq_main_region_Init_Init_Process_Requesting_state_Occupation_default();
			void enseq_main_region_Init_Init_Process_Requesting_state_Wait_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_Operating_Processing_Unlocked_default();
			void enseq_main_region_Operating_Processing_Locked_Route_active_Enabled_default();
			void enseq_main_region_Operating_Processing_Locked_Route_active_Passed_default();
			void enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left_default();
			void enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling_default();
			void enseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling_default();
			void enseq_main_region_Operating_Processing_Locked_Route_active_Disabled_default();
			void enseq_main_region_Operating_Processing_Locked_Route_active_Wait_for_Unlock_default();
			void enseq_main_region_Operating_Processing_Locked_Occupation_Free_default();
			void enseq_main_region_Operating_Processing_Locked_Occupation_Occupied_default();
			void enseq_main_region_Operating_Processing_Locked_Occupation__final__default();
			void enseq_main_region_Operating_Processing_Locked_Occupation_Next_Reached_default();
			void enseq_main_region_Operating_Processing_Pending_Relais_processing_Enabling_default();
			void enseq_main_region_Operating_Processing_Pending_Relais_processing_Disabling_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_Wait_for_Start_default();
			void enseq_main_region_default();
			void enseq_main_region_Init_Init_Process_default();
			void enseq_main_region_Operating_Processing_default();
			void enseq_main_region_Operating_Processing_Locked_Occupation_default();
			void exseq_main_region_Init();
			void exseq_main_region_Init_Init_Process_Requesting();
			void exseq_main_region_Init_Init_Process_Requesting_relais_Relay();
			void exseq_main_region_Init_Init_Process_Requesting_relais_Wait();
			void exseq_main_region_Init_Init_Process_Requesting_state_Occupation();
			void exseq_main_region_Init_Init_Process_Requesting_state_Wait();
			void exseq_main_region_Operating();
			void exseq_main_region_Operating_Processing_Unlocked();
			void exseq_main_region_Operating_Processing_Locked();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Enabled();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Passed();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Waiting();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Disabled();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Wait_for_Unlock();
			void exseq_main_region_Operating_Processing_Locked_Occupation_Free();
			void exseq_main_region_Operating_Processing_Locked_Occupation_Occupied();
			void exseq_main_region_Operating_Processing_Locked_Occupation__final_();
			void exseq_main_region_Operating_Processing_Locked_Occupation_Next_Reached();
			void exseq_main_region_Operating_Processing_Pending();
			void exseq_main_region_Operating_Processing_Pending_Relais_processing_Enabling();
			void exseq_main_region_Operating_Processing_Pending_Relais_processing_Disabling();
			void exseq_main_region_Failed();
			void exseq_main_region_Wait_for_Start();
			void exseq_main_region();
			void exseq_main_region_Init_Init_Process();
			void exseq_main_region_Init_Init_Process_Requesting_relais();
			void exseq_main_region_Init_Init_Process_Requesting_state();
			void exseq_main_region_Operating_Processing();
			void exseq_main_region_Operating_Processing_Locked_Route_active();
			void exseq_main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing();
			void exseq_main_region_Operating_Processing_Locked_Occupation();
			void exseq_main_region_Operating_Processing_Pending_Relais_processing();
			void react_main_region_Operating_Processing_Locked_Route_active__choice_0();
			void react_main_region_Operating_Processing_Locked_Occupation__choice_0();
			void react_main_region__entry_Default();
			void react_main_region_Init_Init_Process__entry_Default();
			void react_main_region_Operating_Processing__entry_Default();
			void react_main_region_Operating_Processing_Locked_Occupation__entry_Default();
			void react_main_region_Init_Init_Process__sync0();
			void react_main_region_Init_Init_Process__sync1();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_relais_Relay_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_relais_Wait_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_state_Occupation_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_state_Wait_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Unlocked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Route_active_Enabled_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Route_active_Passed_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Route_active_Waiting_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Left_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Disabling_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Route_active_Waiting_Relais_processing_Enabling_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Route_active_Disabled_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Route_active_Wait_for_Unlock_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Occupation_Free_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Occupation_Occupied_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Occupation__final__react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Occupation_Next_Reached_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Relais_processing_Enabling_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Relais_processing_Disabling_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_for_Start_react(const sc::integer transitioned_before);
			void clearOutEvents() noexcept;
			void clearInEvents() noexcept;
			void clearInternalEvents() noexcept;
			void microStep();
			void runCycle();



			/*! Indicates event 'enable' of default interface scope is active. */
			bool enable_raised {false};

			/*! Value of event 'enable' of default interface scope. */
			bool enable_value {false};

			/*! Indicates event 'disable' of default interface scope is active. */
			bool disable_raised {false};

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised {false};

			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised {false};

			/*! Indicates event 'relaisResponse' of default interface scope is active. */
			bool relaisResponse_raised {false};

			/*! Indicates event 'stateResponse' of default interface scope is active. */
			bool stateResponse_raised {false};

			/*! Value of event 'stateResponse' of default interface scope. */
			bool stateResponse_value {false};

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised {false};

			/*! Indicates event 'next' of default interface scope is active. */
			bool next_raised {false};

			/*! Indicates event 'unlock' of default interface scope is active. */
			bool unlock_raised {false};

			/*! Indicates event 'started' of default interface scope is active. */
			bool started_raised {false};

			/*! Indicates event 'stop' of default interface scope is active. */
			bool stop_raised {false};

			/*! Indicates event 'entered' of default interface scope is active. */
			bool entered_raised {false};

			/*! Indicates event 'leave' of default interface scope is active. */
			bool leave_raised {false};

			/*! Indicates event 'leaving' of default interface scope is active. */
			bool leaving_raised {false};

			/*! Indicates event 'left' of default interface scope is active. */
			bool left_raised {false};

			/*! Indicates event 'unregister' of default interface scope is active. */
			bool unregister_raised {false};

			/*! Indicates event 'local_leave' of internal scope is active. */
			bool local_leave_raised {false};

			/*! Raises the out event 'local_leave' of internal scope as a local event. */
			void raiseLocal_leave();



		};


		inline SectionStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SECTIONSTATECHART_H_ */
