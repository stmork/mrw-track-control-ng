/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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
				main_region_Disable,
				main_region_Start,
				main_region__final_,
				main_region_Active,
				main_region_Active_processing_Switch_Turning,
				main_region_Active_processing_Signal_Turning,
				main_region_Active_processing_Section_Activation,
				main_region_Active_processing_Signal_Updating,
				main_region_Active_processing_Flank_Turning,
				main_region_Active_processing_Completed,
				main_region_Wait,
				main_region_Emergency_Shutdown,
				main_region_Unlock
			};

			/*! The number of states. */
			static const sc::integer numStates = 13;
			static const sc::integer scvi_main_region_Disable = 0;
			static const sc::integer scvi_main_region_Start = 0;
			static const sc::integer scvi_main_region__final_ = 0;
			static const sc::integer scvi_main_region_Active = 0;
			static const sc::integer scvi_main_region_Active_processing_Switch_Turning = 0;
			static const sc::integer scvi_main_region_Active_processing_Signal_Turning = 0;
			static const sc::integer scvi_main_region_Active_processing_Section_Activation = 0;
			static const sc::integer scvi_main_region_Active_processing_Signal_Updating = 0;
			static const sc::integer scvi_main_region_Active_processing_Flank_Turning = 0;
			static const sc::integer scvi_main_region_Active_processing_Completed = 0;
			static const sc::integer scvi_main_region_Wait = 0;
			static const sc::integer scvi_main_region_Emergency_Shutdown = 0;
			static const sc::integer scvi_main_region_Unlock = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				turn,
				completed,
				failed,
				disable,
				_te0_main_region_Disable_,
				_te1_main_region_Active_processing_Switch_Turning_,
				_te2_main_region_Active_processing_Signal_Turning_,
				_te3_main_region_Active_processing_Section_Activation_,
				_te4_main_region_Active_processing_Signal_Updating_,
				_te5_main_region_Active_processing_Flank_Turning_,
				_te6_main_region_Wait_,
				_te7_main_region_Emergency_Shutdown_
			};

			class EventInstance
			{
			public:
				explicit EventInstance(Event id) : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			/*! Raises the in event 'turn' of default interface scope. */
			void raiseTurn();
			/*! Raises the in event 'completed' of default interface scope. */
			void raiseCompleted();
			/*! Raises the in event 'failed' of default interface scope. */
			void raiseFailed();
			/*! Raises the in event 'disable' of default interface scope. */
			void raiseDisable();
			/*! Check if event 'activated' of default interface scope is raised. */
			bool isRaisedActivated();
			/*! Check if event 'finished' of default interface scope is raised. */
			bool isRaisedFinished();

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent();

			/*! Gets the value of the variable 'switch_timeout' that is defined in the default interface scope. */
			static sc::integer getSwitch_timeout() ;

			/*! Gets the value of the variable 'signal_timeout' that is defined in the default interface scope. */
			static sc::integer getSignal_timeout() ;

			/*! Gets the value of the variable 'section_timeout' that is defined in the default interface scope. */
			static sc::integer getSection_timeout() ;

			/*! Gets the value of the variable 'emergency' that is defined in the default interface scope. */
			static sc::integer getEmergency() ;

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void prepareRoute() = 0;

				virtual void prepareFlank() = 0;

				virtual bool isTour() = 0;

				virtual bool isCompleted() = 0;

				virtual void resetTransaction() = 0;

				virtual void fail() = 0;

				virtual void tryComplete() = 0;

				virtual void turnSwitches() = 0;

				virtual void turnFlanks() = 0;

				virtual void enableSignals() = 0;

				virtual void extendSignals() = 0;

				virtual void enableSections() = 0;

				virtual void disableSections() = 0;

				virtual void disableSignals() = 0;

				virtual void unlockFlanks() = 0;

				virtual void unlockRailParts() = 0;

				virtual void unlockSections() = 0;


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
			static const sc::integer timeEventsCount = 8;

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount = 1;


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			RouteStatechart(const RouteStatechart & rhs);
			RouteStatechart & operator=(const RouteStatechart &);

			static const sc::integer switch_timeout;
			static const sc::integer signal_timeout;
			static const sc::integer section_timeout;
			static const sc::integer emergency;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;


			// prototypes of all internal functions

			void enact_main_region_Disable();
			void enact_main_region_Active_processing_Switch_Turning();
			void enact_main_region_Active_processing_Signal_Turning();
			void enact_main_region_Active_processing_Section_Activation();
			void enact_main_region_Active_processing_Signal_Updating();
			void enact_main_region_Active_processing_Flank_Turning();
			void enact_main_region_Active_processing_Completed();
			void enact_main_region_Wait();
			void enact_main_region_Emergency_Shutdown();
			void enact_main_region_Unlock();
			void exact_main_region_Disable();
			void exact_main_region_Active_processing_Switch_Turning();
			void exact_main_region_Active_processing_Signal_Turning();
			void exact_main_region_Active_processing_Section_Activation();
			void exact_main_region_Active_processing_Signal_Updating();
			void exact_main_region_Active_processing_Flank_Turning();
			void exact_main_region_Wait();
			void exact_main_region_Emergency_Shutdown();
			void enseq_main_region_Disable_default();
			void enseq_main_region_Start_default();
			void enseq_main_region__final__default();
			void enseq_main_region_Active_processing_Switch_Turning_default();
			void enseq_main_region_Active_processing_Signal_Turning_default();
			void enseq_main_region_Active_processing_Section_Activation_default();
			void enseq_main_region_Active_processing_Signal_Updating_default();
			void enseq_main_region_Active_processing_Flank_Turning_default();
			void enseq_main_region_Active_processing_Completed_default();
			void enseq_main_region_Wait_default();
			void enseq_main_region_Emergency_Shutdown_default();
			void enseq_main_region_Unlock_default();
			void enseq_main_region_default();
			void exseq_main_region_Disable();
			void exseq_main_region_Start();
			void exseq_main_region__final_();
			void exseq_main_region_Active();
			void exseq_main_region_Active_processing_Switch_Turning();
			void exseq_main_region_Active_processing_Signal_Turning();
			void exseq_main_region_Active_processing_Section_Activation();
			void exseq_main_region_Active_processing_Signal_Updating();
			void exseq_main_region_Active_processing_Flank_Turning();
			void exseq_main_region_Active_processing_Completed();
			void exseq_main_region_Wait();
			void exseq_main_region_Emergency_Shutdown();
			void exseq_main_region_Unlock();
			void exseq_main_region();
			void exseq_main_region_Active_processing();
			void react_main_region_Active_processing__choice_0();
			void react_main_region__choice_0();
			void react_main_region__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Disable_react(const sc::integer transitioned_before);
			sc::integer main_region_Start_react(const sc::integer transitioned_before);
			sc::integer main_region__final__react(const sc::integer transitioned_before);
			sc::integer main_region_Active_react(const sc::integer transitioned_before);
			sc::integer main_region_Active_processing_Switch_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Active_processing_Signal_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Active_processing_Section_Activation_react(const sc::integer transitioned_before);
			sc::integer main_region_Active_processing_Signal_Updating_react(const sc::integer transitioned_before);
			sc::integer main_region_Active_processing_Flank_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Active_processing_Completed_react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_react(const sc::integer transitioned_before);
			sc::integer main_region_Emergency_Shutdown_react(const sc::integer transitioned_before);
			sc::integer main_region_Unlock_react(const sc::integer transitioned_before);
			void clearOutEvents();
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'turn' of default interface scope is active. */
			bool turn_raised;

			/*! Indicates event 'completed' of default interface scope is active. */
			bool completed_raised;

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised;

			/*! Indicates event 'disable' of default interface scope is active. */
			bool disable_raised;

			/*! Indicates event 'activated' of default interface scope is active. */
			bool activated_raised;

			/*! Indicates event 'finished' of default interface scope is active. */
			bool finished_raised;



		};


		inline RouteStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* ROUTESTATECHART_H_ */
