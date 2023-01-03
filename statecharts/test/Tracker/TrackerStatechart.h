/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#ifndef TRACKERSTATECHART_H_
#define TRACKERSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the TrackerStatechart state machine.
		*/
		class TrackerStatechart;
	}
}


#include <deque>
#include "../common/sc_types.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_timer.h"

/*! \file
Header of the state machine 'TrackerStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class TrackerStatechart : public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
		public:
			TrackerStatechart();

			virtual ~TrackerStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Preparing,
				main_region_Driving,
				main_region_Driving_Tracking_First,
				main_region_Driving_Tracking_Occupy,
				main_region_Driving_Tracking_Free,
				main_region_Idle
			};

			/*! The number of states. */
			static const sc::integer numStates = 6;
			static const sc::integer scvi_main_region_Preparing = 0;
			static const sc::integer scvi_main_region_Driving = 0;
			static const sc::integer scvi_main_region_Driving_Tracking_First = 0;
			static const sc::integer scvi_main_region_Driving_Tracking_Occupy = 0;
			static const sc::integer scvi_main_region_Driving_Tracking_Free = 0;
			static const sc::integer scvi_main_region_Idle = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				received,
				Internal_completed,
				_te0_main_region_Preparing_,
				_te1_main_region_Driving_Tracking_First_,
				_te2_main_region_Driving_Tracking_Occupy_,
				_te3_main_region_Driving_Tracking_Free_
			};

			class EventInstance
			{
			public:
				explicit EventInstance(Event id) : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			/*! Raises the in event 'received' of default interface scope. */
			void raiseReceived();

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent();

			/*! Gets the value of the variable 'start' that is defined in the default interface scope. */
			sc::integer getStart() const;

			/*! Sets the value of the variable 'start' that is defined in the default interface scope. */
			void setStart(sc::integer start);

			/*! Gets the value of the variable 'step' that is defined in the default interface scope. */
			sc::integer getStep() const;

			/*! Sets the value of the variable 'step' that is defined in the default interface scope. */
			void setStep(sc::integer step);

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void first() = 0;

				virtual void free() = 0;

				virtual void occupy() = 0;

				virtual bool valid() = 0;

				virtual bool last() = 0;

				virtual void clear() = 0;


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
			static const sc::integer timeEventsCount = 4;

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount = 1;


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			std::deque<EventInstance *> internalEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			TrackerStatechart(const TrackerStatechart & rhs);
			TrackerStatechart & operator=(const TrackerStatechart &);

			sc::integer start;
			sc::integer step;



			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;


			// prototypes of all internal functions

			void enact_main_region_Preparing();
			void enact_main_region_Driving_Tracking_First();
			void enact_main_region_Driving_Tracking_Occupy();
			void enact_main_region_Driving_Tracking_Free();
			void enact_main_region_Idle();
			void exact_main_region_Preparing();
			void exact_main_region_Driving_Tracking_First();
			void exact_main_region_Driving_Tracking_Occupy();
			void exact_main_region_Driving_Tracking_Free();
			void enseq_main_region_Preparing_default();
			void enseq_main_region_Driving_default();
			void enseq_main_region_Driving_Tracking_First_default();
			void enseq_main_region_Driving_Tracking_Occupy_default();
			void enseq_main_region_Driving_Tracking_Free_default();
			void enseq_main_region_Idle_default();
			void enseq_main_region_default();
			void enseq_main_region_Driving_Tracking_default();
			void exseq_main_region_Preparing();
			void exseq_main_region_Driving();
			void exseq_main_region_Driving_Tracking_First();
			void exseq_main_region_Driving_Tracking_Occupy();
			void exseq_main_region_Driving_Tracking_Free();
			void exseq_main_region_Idle();
			void exseq_main_region();
			void exseq_main_region_Driving_Tracking();
			void react_main_region__choice_0();
			void react_main_region__entry_Default();
			void react_main_region_Driving_Tracking__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Preparing_react(const sc::integer transitioned_before);
			sc::integer main_region_Driving_react(const sc::integer transitioned_before);
			sc::integer main_region_Driving_Tracking_First_react(const sc::integer transitioned_before);
			sc::integer main_region_Driving_Tracking_Occupy_react(const sc::integer transitioned_before);
			sc::integer main_region_Driving_Tracking_Free_react(const sc::integer transitioned_before);
			sc::integer main_region_Idle_react(const sc::integer transitioned_before);
			void clearInEvents();
			void clearInternalEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'received' of default interface scope is active. */
			bool received_raised;

			/*! Indicates event 'completed' of internal scope is active. */
			bool completed_raised;



		};


		inline TrackerStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* TRACKERSTATECHART_H_ */
