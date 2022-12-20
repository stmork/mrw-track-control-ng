/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
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
#include "common/sc_types.h"
#include "common/sc_statemachine.h"
#include "common/sc_timer.h"
#include <QObject>

/*! \file
Header of the state machine 'OperatingMode'.
*/

namespace mrw
{
	namespace statechart
	{

		class OperatingMode : public QObject, public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
			Q_OBJECT

		public:
			OperatingMode(QObject * parent);

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
				fail,
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

				virtual void reset() = 0;


			};

			/*! Set the working instance of the operation callback interface 'OperationCallback'. */
			void setOperationCallback(OperationCallback * operationCallback);
			//! Inner class for can interface scope.
			class Can
			{
			public:
				Can(OperatingMode * parent);




				//! Inner class for can interface scope operation callbacks.
				class OperationCallback
				{
				public:
					virtual ~OperationCallback() = 0;

					virtual void connectBus() = 0;


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


		public slots:
			/*! Slot for the in event 'clear' that is defined in the default interface scope. */
			void clear();

			/*! Slot for the in event 'started' that is defined in the default interface scope. */
			void started();

			/*! Slot for the in event 'fail' that is defined in the default interface scope. */
			void fail();

			/*! Slot for the in event 'edit' that is defined in the default interface scope. */
			void edit();

			/*! Slot for the in event 'operate' that is defined in the default interface scope. */
			void operate();

			/*! Slot for the in event 'init' that is defined in the default interface scope. */
			void init();

			/*! Slot for the in event 'connected' that is defined in the interface scope 'can'. */
			void can_connected();


		signals:
			/*! Signal representing the out event 'start' that is defined in the default interface scope. */
			void start();

			/*! Signal representing the out event 'failed' that is defined in the default interface scope. */
			void failed();

			/*! Signal representing the out event 'cleared' that is defined in the default interface scope. */
			void cleared();

			/*! Signal representing the out event 'operating' that is defined in the default interface scope. */
			void operating(bool value);

			/*! Signal representing the out event 'editing' that is defined in the default interface scope. */
			void editing(bool value);


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
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised;

			/*! Indicates event 'started' of default interface scope is active. */
			bool started_raised;

			/*! Indicates event 'fail' of default interface scope is active. */
			bool fail_raised;

			/*! Indicates event 'edit' of default interface scope is active. */
			bool edit_raised;

			/*! Indicates event 'operate' of default interface scope is active. */
			bool operate_raised;

			/*! Indicates event 'init' of default interface scope is active. */
			bool init_raised;

			/*! Value of event 'operating' of default interface scope. */
			bool operating_value;

			/*! Value of event 'editing' of default interface scope. */
			bool editing_value;



		};


		inline OperatingMode::OperationCallback::~OperationCallback() {}
		inline OperatingMode::Can::OperationCallback::~OperationCallback() {}

	}
}

#endif /* OPERATINGMODE_H_ */
