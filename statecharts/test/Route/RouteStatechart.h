/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#ifndef ROUTESTATECHART_H_
#define ROUTESTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the RouteStatechart state machine.
		*/
		class RouteStatechart;
	}
}


#include <deque>
#include "../common/sc_types.h"
#include "../common/sc_rxcpp.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_timer.h"

/*! \file
Header of the state machine 'RouteStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class RouteStatechart : public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
		public:
			RouteStatechart();

			virtual ~RouteStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Active,
				main_region_Disable,
				main_region_Start,
				main_region__final_,
				main_region_Turning,
				main_region_Turning_Turning_process_Switch_Turning,
				main_region_Turning_Turning_process_Signal_Turning,
				main_region_Turning_Turning_process_Section_Activation,
				main_region_Turning_Turning_process_Signal_Updating,
				main_region_Wait,
				main_region_Emergency_Shutdown
			};

			/*! The number of states. */
			static const sc::integer numStates = 11;
			static const sc::integer scvi_main_region_Active = 0;
			static const sc::integer scvi_main_region_Disable = 0;
			static const sc::integer scvi_main_region_Start = 0;
			static const sc::integer scvi_main_region__final_ = 0;
			static const sc::integer scvi_main_region_Turning = 0;
			static const sc::integer scvi_main_region_Turning_Turning_process_Switch_Turning = 0;
			static const sc::integer scvi_main_region_Turning_Turning_process_Signal_Turning = 0;
			static const sc::integer scvi_main_region_Turning_Turning_process_Section_Activation = 0;
			static const sc::integer scvi_main_region_Turning_Turning_process_Signal_Updating = 0;
			static const sc::integer scvi_main_region_Wait = 0;
			static const sc::integer scvi_main_region_Emergency_Shutdown = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				completed,
				failed,
				disable,
				extended,
				_te0_main_region_Disable_,
				_te1_main_region_Turning_Turning_process_Switch_Turning_,
				_te2_main_region_Turning_Turning_process_Signal_Turning_,
				_te3_main_region_Turning_Turning_process_Section_Activation_,
				_te4_main_region_Turning_Turning_process_Signal_Updating_,
				_te5_main_region_Wait_,
				_te6_main_region_Emergency_Shutdown_
			};

			class EventInstance
			{
			public:
				explicit EventInstance(Event id) : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			/*! Raises the in event 'completed' of default interface scope. */
			void raiseCompleted();
			/*! Raises the in event 'failed' of default interface scope. */
			void raiseFailed();
			/*! Raises the in event 'disable' of default interface scope. */
			void raiseDisable();
			/*! Raises the in event 'extended' of default interface scope. */
			void raiseExtended();
			/*! Check if event 'activated' of default interface scope is raised. */
			bool isRaisedActivated();
			/*! Get observable for event 'activated' of default interface scope. */
			sc::rx::Observable<void> * getActivated();
			/*! Check if event 'finished' of default interface scope is raised. */
			bool isRaisedFinished();
			/*! Get observable for event 'finished' of default interface scope. */
			sc::rx::Observable<void> * getFinished();

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent();

			/*! Gets the value of the variable 'switch_timeout' that is defined in the default interface scope. */
			sc::integer getSwitch_timeout() const;

			/*! Sets the value of the variable 'switch_timeout' that is defined in the default interface scope. */
			void setSwitch_timeout(sc::integer switch_timeout);

			/*! Gets the value of the variable 'signal_timeout' that is defined in the default interface scope. */
			sc::integer getSignal_timeout() const;

			/*! Sets the value of the variable 'signal_timeout' that is defined in the default interface scope. */
			void setSignal_timeout(sc::integer signal_timeout);

			/*! Gets the value of the variable 'section_timeout' that is defined in the default interface scope. */
			sc::integer getSection_timeout() const;

			/*! Sets the value of the variable 'section_timeout' that is defined in the default interface scope. */
			void setSection_timeout(sc::integer section_timeout);

			/*! Gets the value of the variable 'emergency' that is defined in the default interface scope. */
			sc::integer getEmergency() const;

			/*! Sets the value of the variable 'emergency' that is defined in the default interface scope. */
			void setEmergency(sc::integer emergency);

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void resetTransaction() = 0;

				virtual void fail() = 0;

				virtual void tryComplete() = 0;

				virtual bool isCompleted() = 0;

				virtual void turnSwitches() = 0;

				virtual void activateSections() = 0;

				virtual void turnSignals() = 0;

				virtual void extendSignals() = 0;

				virtual void deactivateSections() = 0;

				virtual void unlockSignals() = 0;

				virtual void unlockSwitches() = 0;


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
			static const sc::integer timeEventsCount = 7;

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount = 1;


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			RouteStatechart(const RouteStatechart & rhs);
			RouteStatechart & operator=(const RouteStatechart &);

			sc::integer switch_timeout;
			sc::integer signal_timeout;
			sc::integer section_timeout;
			sc::integer emergency;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;


			// prototypes of all internal functions

			void enact_main_region_Active();
			void enact_main_region_Disable();
			void enact_main_region_Turning_Turning_process_Switch_Turning();
			void enact_main_region_Turning_Turning_process_Signal_Turning();
			void enact_main_region_Turning_Turning_process_Section_Activation();
			void enact_main_region_Turning_Turning_process_Signal_Updating();
			void enact_main_region_Wait();
			void enact_main_region_Emergency_Shutdown();
			void exact_main_region_Disable();
			void exact_main_region_Turning_Turning_process_Switch_Turning();
			void exact_main_region_Turning_Turning_process_Signal_Turning();
			void exact_main_region_Turning_Turning_process_Section_Activation();
			void exact_main_region_Turning_Turning_process_Signal_Updating();
			void exact_main_region_Wait();
			void exact_main_region_Emergency_Shutdown();
			void enseq_main_region_Active_default();
			void enseq_main_region_Disable_default();
			void enseq_main_region_Start_default();
			void enseq_main_region__final__default();
			void enseq_main_region_Turning_Turning_process_Switch_Turning_default();
			void enseq_main_region_Turning_Turning_process_Signal_Turning_default();
			void enseq_main_region_Turning_Turning_process_Section_Activation_default();
			void enseq_main_region_Turning_Turning_process_Signal_Updating_default();
			void enseq_main_region_Wait_default();
			void enseq_main_region_Emergency_Shutdown_default();
			void enseq_main_region_default();
			void exseq_main_region_Active();
			void exseq_main_region_Disable();
			void exseq_main_region_Start();
			void exseq_main_region__final_();
			void exseq_main_region_Turning();
			void exseq_main_region_Turning_Turning_process_Switch_Turning();
			void exseq_main_region_Turning_Turning_process_Signal_Turning();
			void exseq_main_region_Turning_Turning_process_Section_Activation();
			void exseq_main_region_Turning_Turning_process_Signal_Updating();
			void exseq_main_region_Wait();
			void exseq_main_region_Emergency_Shutdown();
			void exseq_main_region();
			void exseq_main_region_Turning_Turning_process();
			void react_main_region__choice_0();
			void react_main_region__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Active_react(const sc::integer transitioned_before);
			sc::integer main_region_Disable_react(const sc::integer transitioned_before);
			sc::integer main_region_Start_react(const sc::integer transitioned_before);
			sc::integer main_region__final__react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_Turning_process_Switch_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_Turning_process_Signal_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_Turning_process_Section_Activation_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_Turning_process_Signal_Updating_react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_react(const sc::integer transitioned_before);
			sc::integer main_region_Emergency_Shutdown_react(const sc::integer transitioned_before);
			void clearOutEvents();
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'completed' of default interface scope is active. */
			bool completed_raised;

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised;

			/*! Indicates event 'disable' of default interface scope is active. */
			bool disable_raised;

			/*! Indicates event 'extended' of default interface scope is active. */
			bool extended_raised;

			/*! Indicates event 'activated' of default interface scope is active. */
			bool activated_raised;

			/*! Observable for event 'activated' of default interface scope. */
			sc::rx::Observable<void> activated_observable;

			/*! Indicates event 'finished' of default interface scope is active. */
			bool finished_raised;

			/*! Observable for event 'finished' of default interface scope. */
			sc::rx::Observable<void> finished_observable;



		};


		inline RouteStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* ROUTESTATECHART_H_ */
