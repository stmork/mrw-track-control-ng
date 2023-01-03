/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#ifndef OPERATINGMODE_H_
#define OPERATINGMODE_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the OperatingMode state machine.
		*/
		class OperatingMode;
	}
}


#include <deque>
#include "../common/sc_types.h"
#include "../common/sc_rxcpp.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_timer.h"

/*! \file
Header of the state machine 'OperatingMode'.
*/

namespace mrw
{
	namespace statechart
	{

		class OperatingMode : public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
		public:
			OperatingMode();

			virtual ~OperatingMode();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Prepare_Bus,
				main_region_Init,
				main_region_Editing,
				main_region_Failed,
				main_region_Operating
			};

			/*! The number of states. */
			static const sc::integer numStates = 5;
			static const sc::integer scvi_main_region_Prepare_Bus = 0;
			static const sc::integer scvi_main_region_Init = 0;
			static const sc::integer scvi_main_region_Editing = 0;
			static const sc::integer scvi_main_region_Failed = 0;
			static const sc::integer scvi_main_region_Operating = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				clear,
				started,
				failed,
				edit,
				operate,
				init,
				Can_connected,
				_te0_main_region_Prepare_Bus_,
				_te1_main_region_Init_
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
			/*! Raises the in event 'started' of default interface scope. */
			void raiseStarted();
			/*! Raises the in event 'failed' of default interface scope. */
			void raiseFailed();
			/*! Raises the in event 'edit' of default interface scope. */
			void raiseEdit();
			/*! Raises the in event 'operate' of default interface scope. */
			void raiseOperate();
			/*! Raises the in event 'init' of default interface scope. */
			void raiseInit();
			/*! Check if event 'start' of default interface scope is raised. */
			bool isRaisedStart();
			/*! Get observable for event 'start' of default interface scope. */
			sc::rx::Observable<void> * getStart();
			/*! Check if event 'cleared' of default interface scope is raised. */
			bool isRaisedCleared();
			/*! Get observable for event 'cleared' of default interface scope. */
			sc::rx::Observable<void> * getCleared();
			/*! Check if event 'failing' of default interface scope is raised. */
			bool isRaisedFailing();
			/*! Get observable for event 'failing' of default interface scope. */
			sc::rx::Observable<void> * getFailing();
			/*! Check if event 'operating' of default interface scope is raised. */
			bool isRaisedOperating();
			/*! Get value of event 'operating' of default interface scope. */
			bool getOperatingValue();
			/*! Get observable for event 'operating' of default interface scope. */
			sc::rx::Observable<bool> * getOperating();
			/*! Check if event 'editing' of default interface scope is raised. */
			bool isRaisedEditing();
			/*! Get value of event 'editing' of default interface scope. */
			bool getEditingValue();
			/*! Get observable for event 'editing' of default interface scope. */
			sc::rx::Observable<bool> * getEditing();

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent();

			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			sc::integer getTimeout() const;

			/*! Sets the value of the variable 'timeout' that is defined in the default interface scope. */
			void setTimeout(sc::integer timeout);

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void resetTransaction() = 0;

				virtual bool hasActiveRoutes() = 0;


			};

			/*! Set the working instance of the operation callback interface 'OperationCallback'. */
			void setOperationCallback(OperationCallback * operationCallback);
			//! Inner class for can interface scope.
			class Can
			{
			public:
				Can(OperatingMode * parent);




				/*! Raises the in event 'connected' of interface scope 'can'. */
				void raiseConnected();

				//! Inner class for can interface scope operation callbacks.
				class OperationCallback
				{
				public:
					virtual ~OperationCallback() = 0;

					virtual void connectBus() = 0;

					virtual bool isConnected() = 0;


				};

				/*! Set the working instance of the operation callback interface 'OperationCallback'. */
				void setOperationCallback(OperationCallback * operationCallback);


			private:
				friend class OperatingMode;

				/*! Indicates event 'connected' of interface scope 'can' is active. */
				bool connected_raised;

				OperatingMode * parent;



				OperationCallback * ifaceCanOperationCallback;


			};

			/*! Returns an instance of the interface class 'Can'. */
			Can * can();


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
			OperatingMode(const OperatingMode & rhs);
			OperatingMode & operator=(const OperatingMode &);

			sc::integer timeout;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			Can ifaceCan;
			OperationCallback * ifaceOperationCallback;


			bool isExecuting;


			// prototypes of all internal functions

			void enact_main_region_Prepare_Bus();
			void enact_main_region_Init();
			void enact_main_region_Editing();
			void enact_main_region_Failed();
			void enact_main_region_Operating();
			void exact_main_region_Prepare_Bus();
			void exact_main_region_Init();
			void exact_main_region_Editing();
			void exact_main_region_Failed();
			void exact_main_region_Operating();
			void enseq_main_region_Prepare_Bus_default();
			void enseq_main_region_Init_default();
			void enseq_main_region_Editing_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_default();
			void exseq_main_region_Prepare_Bus();
			void exseq_main_region_Init();
			void exseq_main_region_Editing();
			void exseq_main_region_Failed();
			void exseq_main_region_Operating();
			void exseq_main_region();
			void react_main_region__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Prepare_Bus_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Editing_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_react(const sc::integer transitioned_before);
			void clearOutEvents();
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised;

			/*! Indicates event 'started' of default interface scope is active. */
			bool started_raised;

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised;

			/*! Indicates event 'edit' of default interface scope is active. */
			bool edit_raised;

			/*! Indicates event 'operate' of default interface scope is active. */
			bool operate_raised;

			/*! Indicates event 'init' of default interface scope is active. */
			bool init_raised;

			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised;

			/*! Observable for event 'start' of default interface scope. */
			sc::rx::Observable<void> start_observable;

			/*! Indicates event 'cleared' of default interface scope is active. */
			bool cleared_raised;

			/*! Observable for event 'cleared' of default interface scope. */
			sc::rx::Observable<void> cleared_observable;

			/*! Indicates event 'failing' of default interface scope is active. */
			bool failing_raised;

			/*! Observable for event 'failing' of default interface scope. */
			sc::rx::Observable<void> failing_observable;

			/*! Value of event 'operating' of default interface scope. */
			bool operating_value;

			/*! Indicates event 'operating' of default interface scope is active. */
			bool operating_raised;

			/*! Observable for event 'operating' of default interface scope. */
			sc::rx::Observable<bool> operating_observable;

			/*! Value of event 'editing' of default interface scope. */
			bool editing_value;

			/*! Indicates event 'editing' of default interface scope is active. */
			bool editing_raised;

			/*! Observable for event 'editing' of default interface scope. */
			sc::rx::Observable<bool> editing_observable;



		};


		inline OperatingMode::OperationCallback::~OperationCallback() {}
		inline OperatingMode::Can::OperationCallback::~OperationCallback() {}

	}
}

#endif /* OPERATINGMODE_H_ */
