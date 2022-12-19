/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//
* */

#ifndef SIGNALSTATECHART_H_
#define SIGNALSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the SignalStatechart state machine.
		*/
		class SignalStatechart;
	}
}


#include <deque>
#include "common/sc_types.h"
#include "common/sc_statemachine.h"
#include "common/sc_timer.h"
#include <QObject>

/*! \file
Header of the state machine 'SignalStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SignalStatechart : public QObject, public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
			Q_OBJECT

		public:
			SignalStatechart(QObject * parent);

			virtual ~SignalStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Idle,
				main_region_Turning,
				main_region_Turning_Turn_processing_Pending,
				main_region_Turning_Turn_processing_Turn,
				main_region_Fail
			};

			/*! The number of states. */
			static const sc::integer numStates = 5;
			static const sc::integer scvi_main_region_Idle = 0;
			static const sc::integer scvi_main_region_Turning = 0;
			static const sc::integer scvi_main_region_Turning_Turn_processing_Pending = 0;
			static const sc::integer scvi_main_region_Turning_Turn_processing_Turn = 0;
			static const sc::integer scvi_main_region_Fail = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				turn,
				queued,
				ok,
				fail,
				clear,
				_te0_main_region_Turning_
			};

			class EventInstance
			{
			public:
				explicit EventInstance(Event id) : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			template <typename T>
			class EventInstanceWithValue : public EventInstance
			{
			public:
				explicit EventInstanceWithValue(Event id, T val) :
					EventInstance(id),
					value(val)
				{}
				virtual ~EventInstanceWithValue() = default;
				const T value;
			};

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent();

			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			sc::integer getTimeout() const;

			/*! Sets the value of the variable 'timeout' that is defined in the default interface scope. */
			void setTimeout(sc::integer timeout);

			/*! Gets the value of the variable 'signalState' that is defined in the default interface scope. */
			sc::integer getSignalState() const;

			/*! Sets the value of the variable 'signalState' that is defined in the default interface scope. */
			void setSignalState(sc::integer signalState);

			/*! Gets the value of the variable 'sectionState' that is defined in the default interface scope. */
			sc::integer getSectionState() const;

			/*! Sets the value of the variable 'sectionState' that is defined in the default interface scope. */
			void setSectionState(sc::integer sectionState);

			/*! Gets the value of the variable 'FREE' that is defined in the default interface scope. */
			static sc::integer getFREE() ;

			/*! Gets the value of the variable 'SHUNTING' that is defined in the default interface scope. */
			static sc::integer getSHUNTING() ;

			/*! Gets the value of the variable 'TOUR' that is defined in the default interface scope. */
			static sc::integer getTOUR() ;

			/*! Gets the value of the variable 'OFF' that is defined in the default interface scope. */
			static sc::integer getOFF() ;

			/*! Gets the value of the variable 'STOP' that is defined in the default interface scope. */
			static sc::integer getSTOP() ;

			/*! Gets the value of the variable 'GO' that is defined in the default interface scope. */
			static sc::integer getGO() ;

			/*! Gets the value of the variable 'SLOW' that is defined in the default interface scope. */
			static sc::integer getSLOW() ;

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual bool hasSignal() = 0;

				virtual void send(sc::integer symbol) = 0;

				virtual void idle() = 0;


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
			static const sc::integer timeEventsCount = 1;

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount = 1;


		public slots:
			/*! Slot for the in event 'turn' that is defined in the default interface scope. */
			void turn(sc::integer turn_);

			/*! Slot for the in event 'queued' that is defined in the default interface scope. */
			void queued();

			/*! Slot for the in event 'ok' that is defined in the default interface scope. */
			void ok();

			/*! Slot for the in event 'fail' that is defined in the default interface scope. */
			void fail();

			/*! Slot for the in event 'clear' that is defined in the default interface scope. */
			void clear();


		signals:
			/*! Signal representing the out event 'completed' that is defined in the default interface scope. */
			void completed();

			/*! Signal representing the out event 'failed' that is defined in the default interface scope. */
			void failed();


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			SignalStatechart(const SignalStatechart & rhs);
			SignalStatechart & operator=(const SignalStatechart &);

			sc::integer timeout;
			sc::integer signalState;
			sc::integer sectionState;
			static const sc::integer FREE;
			static const sc::integer SHUNTING;
			static const sc::integer TOUR;
			static const sc::integer OFF;
			static const sc::integer STOP;
			static const sc::integer GO;
			static const sc::integer SLOW;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;


			// prototypes of all internal functions

			void enact_main_region_Idle();
			void enact_main_region_Turning();
			void enact_main_region_Turning_Turn_processing_Turn();
			void enact_main_region_Fail();
			void exact_main_region_Turning();
			void enseq_main_region_Idle_default();
			void enseq_main_region_Turning_default();
			void enseq_main_region_Turning_Turn_processing_Pending_default();
			void enseq_main_region_Turning_Turn_processing_Turn_default();
			void enseq_main_region_Fail_default();
			void enseq_main_region_default();
			void enseq_main_region_Turning_Turn_processing_default();
			void exseq_main_region_Idle();
			void exseq_main_region_Turning();
			void exseq_main_region_Turning_Turn_processing_Pending();
			void exseq_main_region_Turning_Turn_processing_Turn();
			void exseq_main_region_Fail();
			void exseq_main_region();
			void exseq_main_region_Turning_Turn_processing();
			void react_main_region__choice_0();
			void react_main_region__entry_Default();
			void react_main_region_Turning_Turn_processing__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Idle_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_Turn_processing_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_Turn_processing_Turn_react(const sc::integer transitioned_before);
			sc::integer main_region_Fail_react(const sc::integer transitioned_before);
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'turn' of default interface scope is active. */
			bool turn_raised;

			/*! Value of event 'turn' of default interface scope. */
			sc::integer turn_value;

			/*! Indicates event 'queued' of default interface scope is active. */
			bool queued_raised;

			/*! Indicates event 'ok' of default interface scope is active. */
			bool ok_raised;

			/*! Indicates event 'fail' of default interface scope is active. */
			bool fail_raised;

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised;



		};


		inline SignalStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SIGNALSTATECHART_H_ */
