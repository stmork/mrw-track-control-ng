/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#ifndef SWITCHSTATECHART_H_
#define SWITCHSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the SwitchStatechart state machine.
		*/
		class SwitchStatechart;
	}
}


#include <deque>
#include "../common/sc_types.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_timer.h"

/*! \file
Header of the state machine 'SwitchStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SwitchStatechart : public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
		public:
			SwitchStatechart();

			virtual ~SwitchStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Wait_for_Start,
				main_region_Init,
				main_region_Operating,
				main_region_Operating_operating_Unlocked,
				main_region_Operating_operating_Locked,
				main_region_Operating_operating_Turning,
				main_region_Operating_operating_Turning_Turning_process_Turn_Right,
				main_region_Operating_operating_Turning_Turning_process_Turn_Left,
				main_region_Operating_operating_Turning_Turning_process_Pending,
				main_region_Failed
			};

			/*! The number of states. */
			static const sc::integer numStates = 10;
			static const sc::integer scvi_main_region_Wait_for_Start = 0;
			static const sc::integer scvi_main_region_Init = 0;
			static const sc::integer scvi_main_region_Operating = 0;
			static const sc::integer scvi_main_region_Operating_operating_Unlocked = 0;
			static const sc::integer scvi_main_region_Operating_operating_Locked = 0;
			static const sc::integer scvi_main_region_Operating_operating_Turning = 0;
			static const sc::integer scvi_main_region_Operating_operating_Turning_Turning_process_Turn_Right = 0;
			static const sc::integer scvi_main_region_Operating_operating_Turning_Turning_process_Turn_Left = 0;
			static const sc::integer scvi_main_region_Operating_operating_Turning_Turning_process_Pending = 0;
			static const sc::integer scvi_main_region_Failed = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				clear,
				start,
				leftResponse,
				rightResponse,
				response,
				queued,
				failed,
				unlock,
				turn,
				_te0_main_region_Init_,
				_te1_main_region_Operating_operating_Turning_
			};

			class EventInstance
			{
			public:
				explicit EventInstance(Event id) : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			/*! Raises the in event 'clear' of default interface scope. */
			void raiseClear();
			/*! Raises the in event 'start' of default interface scope. */
			void raiseStart();
			/*! Raises the in event 'leftResponse' of default interface scope. */
			void raiseLeftResponse();
			/*! Raises the in event 'rightResponse' of default interface scope. */
			void raiseRightResponse();
			/*! Raises the in event 'response' of default interface scope. */
			void raiseResponse();
			/*! Raises the in event 'queued' of default interface scope. */
			void raiseQueued();
			/*! Raises the in event 'failed' of default interface scope. */
			void raiseFailed();
			/*! Raises the in event 'unlock' of default interface scope. */
			void raiseUnlock();
			/*! Raises the in event 'turn' of default interface scope. */
			void raiseTurn();
			/*! Check if event 'started' of default interface scope is raised. */
			bool isRaisedStarted();
			/*! Check if event 'entered' of default interface scope is raised. */
			bool isRaisedEntered();
			/*! Check if event 'stop' of default interface scope is raised. */
			bool isRaisedStop();

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent();

			/*! Gets the value of the variable 'isManual' that is defined in the default interface scope. */
			bool getIsManual() const;

			/*! Sets the value of the variable 'isManual' that is defined in the default interface scope. */
			void setIsManual(bool isManual);

			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			static sc::integer getTimeout() ;

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void inc() = 0;

				virtual void dec() = 0;

				virtual void left() = 0;

				virtual void right() = 0;

				virtual void request() = 0;

				virtual bool doTurnLeft() = 0;

				virtual bool isFree() = 0;

				virtual void fail() = 0;

				virtual void pending() = 0;

				virtual void lock(bool do_it) = 0;


			};

			/*! Set the working instance of the operation callback interface 'OperationCallback'. */
			void setOperationCallback(OperationCallback * operationCallback);

			/*
			 * Functions inherited from StatemachineInterface
			 */
			void enter() override;

			void exit() override;

			/*!
			 * Checks if the state machine is active (until 2.4.1 this method was used for states).
			 * A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
			 */
			bool isActive() const override;


			/*!
			* Checks if all active states are final.
			* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
			*/
			bool isFinal() const override;

			/*!
			 * Checks if member of the state machine must be set. For example an operation callback.
			 */
			bool check() const;

			/*
			 * Functions inherited from TimedStatemachineInterface
			 */
			void setTimerService(sc::timer::TimerServiceInterface * timerService_) override;

			sc::timer::TimerServiceInterface * getTimerService() override;

			void raiseTimeEvent(sc::eventid event) override;

			sc::integer getNumberOfParallelTimeEvents() override;



			/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
			bool isStateActive(State state) const;

			//! number of time events used by the state machine.
			static const sc::integer timeEventsCount = 2;

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount = 1;


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			SwitchStatechart(const SwitchStatechart & rhs);
			SwitchStatechart & operator=(const SwitchStatechart &);

			bool isManual;
			static const sc::integer timeout;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;


			// prototypes of all internal functions

			void enact_main_region_Init();
			void enact_main_region_Operating_operating_Unlocked();
			void enact_main_region_Operating_operating_Locked();
			void enact_main_region_Operating_operating_Turning();
			void enact_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
			void enact_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
			void enact_main_region_Failed();
			void exact_main_region_Init();
			void exact_main_region_Operating_operating_Turning();
			void enseq_main_region_Wait_for_Start_default();
			void enseq_main_region_Init_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_Operating_operating_Unlocked_default();
			void enseq_main_region_Operating_operating_Locked_default();
			void enseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right_default();
			void enseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left_default();
			void enseq_main_region_Operating_operating_Turning_Turning_process_Pending_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_default();
			void enseq_main_region_Operating_operating_default();
			void exseq_main_region_Wait_for_Start();
			void exseq_main_region_Init();
			void exseq_main_region_Operating();
			void exseq_main_region_Operating_operating_Unlocked();
			void exseq_main_region_Operating_operating_Locked();
			void exseq_main_region_Operating_operating_Turning();
			void exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
			void exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
			void exseq_main_region_Operating_operating_Turning_Turning_process_Pending();
			void exseq_main_region_Failed();
			void exseq_main_region();
			void exseq_main_region_Operating_operating();
			void exseq_main_region_Operating_operating_Turning_Turning_process();
			void react_main_region_Operating_operating__choice_0();
			void react_main_region_Operating_operating_Turning_Turning_process__choice_0();
			void react_main_region__entry_Default();
			void react_main_region_Operating_operating__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_for_Start_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Unlocked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Locked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Turning_Turning_process_Turn_Right_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Turning_Turning_process_Turn_Left_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Turning_Turning_process_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			void clearOutEvents();
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised;

			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised;

			/*! Indicates event 'leftResponse' of default interface scope is active. */
			bool leftResponse_raised;

			/*! Indicates event 'rightResponse' of default interface scope is active. */
			bool rightResponse_raised;

			/*! Indicates event 'response' of default interface scope is active. */
			bool response_raised;

			/*! Indicates event 'queued' of default interface scope is active. */
			bool queued_raised;

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised;

			/*! Indicates event 'unlock' of default interface scope is active. */
			bool unlock_raised;

			/*! Indicates event 'turn' of default interface scope is active. */
			bool turn_raised;

			/*! Indicates event 'started' of default interface scope is active. */
			bool started_raised;

			/*! Indicates event 'entered' of default interface scope is active. */
			bool entered_raised;

			/*! Indicates event 'stop' of default interface scope is active. */
			bool stop_raised;



		};


		inline SwitchStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SWITCHSTATECHART_H_ */
