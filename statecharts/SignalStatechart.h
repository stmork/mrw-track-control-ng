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
				main_region_Init,
				main_region_Init_Init_process_Main_signal,
				main_region_Init_Init_process_Main_signal_Turn_main_Turn,
				main_region_Init_Init_process_Main_signal_Turn_main_Pending,
				main_region_Init_Init_process_Distant_signal,
				main_region_Init_Init_process_Distant_signal_Turn_distant_Turn,
				main_region_Init_Init_process_Distant_signal_Turn_distant_Pending,
				main_region_Init_Init_process_Shunt_signal,
				main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn,
				main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending,
				main_region_Wait_for_Start,
				main_region_Operating,
				main_region_Failed
			};

			/*! The number of states. */
			static const sc::integer numStates = 13;
			static const sc::integer scvi_main_region_Init = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Main_signal = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Main_signal_Turn_main_Turn = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Main_signal_Turn_main_Pending = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Distant_signal = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Shunt_signal = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending = 0;
			static const sc::integer scvi_main_region_Wait_for_Start = 0;
			static const sc::integer scvi_main_region_Operating = 0;
			static const sc::integer scvi_main_region_Failed = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				start,
				queued,
				response,
				fail,
				clear,
				_te0_main_region_Init_
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

			/*! Gets the value of the variable 'STOP' that is defined in the default interface scope. */
			static sc::integer getSTOP() ;

			/*! Gets the value of the variable 'GO' that is defined in the default interface scope. */
			static sc::integer getGO() ;

			/*! Gets the value of the variable 'OFF' that is defined in the default interface scope. */
			static sc::integer getOFF() ;

			/*! Gets the value of the variable 'symbolMain' that is defined in the default interface scope. */
			sc::integer getSymbolMain() const;

			/*! Sets the value of the variable 'symbolMain' that is defined in the default interface scope. */
			void setSymbolMain(sc::integer symbolMain);

			/*! Gets the value of the variable 'symbolDistant' that is defined in the default interface scope. */
			sc::integer getSymbolDistant() const;

			/*! Sets the value of the variable 'symbolDistant' that is defined in the default interface scope. */
			void setSymbolDistant(sc::integer symbolDistant);

			/*! Gets the value of the variable 'symbolShunt' that is defined in the default interface scope. */
			sc::integer getSymbolShunt() const;

			/*! Sets the value of the variable 'symbolShunt' that is defined in the default interface scope. */
			void setSymbolShunt(sc::integer symbolShunt);

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void inc() = 0;

				virtual void dec() = 0;

				virtual bool hasMain() = 0;

				virtual bool hasDistant() = 0;

				virtual bool hasShunt() = 0;

				virtual void turnMainSignal(sc::integer symbol) = 0;

				virtual void turnDistantSignal(sc::integer symbol) = 0;

				virtual void turnShuntSignal(sc::integer symbol) = 0;


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
			/*! Slot for the in event 'start' that is defined in the default interface scope. */
			void start();

			/*! Slot for the in event 'queued' that is defined in the default interface scope. */
			void queued();

			/*! Slot for the in event 'response' that is defined in the default interface scope. */
			void response();

			/*! Slot for the in event 'fail' that is defined in the default interface scope. */
			void fail();

			/*! Slot for the in event 'clear' that is defined in the default interface scope. */
			void clear();


		signals:
			/*! Signal representing the out event 'started' that is defined in the default interface scope. */
			void started();


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			SignalStatechart(const SignalStatechart & rhs);
			SignalStatechart & operator=(const SignalStatechart &);

			sc::integer timeout;
			static const sc::integer STOP;
			static const sc::integer GO;
			static const sc::integer OFF;
			sc::integer symbolMain;
			sc::integer symbolDistant;
			sc::integer symbolShunt;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;
			bool stateConfVectorChanged;


			// prototypes of all internal functions

			void enact_main_region_Init();
			void enact_main_region_Init_Init_process_Main_signal_Turn_main_Turn();
			void enact_main_region_Init_Init_process_Distant_signal();
			void enact_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn();
			void enact_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn();
			void exact_main_region_Init();
			void enseq_main_region_Init_default();
			void enseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn_default();
			void enseq_main_region_Init_Init_process_Main_signal_Turn_main_Pending_default();
			void enseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn_default();
			void enseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending_default();
			void enseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn_default();
			void enseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending_default();
			void enseq_main_region_Wait_for_Start_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_default();
			void enseq_main_region_Init_Init_process_default();
			void exseq_main_region_Init();
			void exseq_main_region_Init_Init_process_Main_signal();
			void exseq_main_region_Init_Init_process_Main_signal_Turn_main_Turn();
			void exseq_main_region_Init_Init_process_Main_signal_Turn_main_Pending();
			void exseq_main_region_Init_Init_process_Distant_signal();
			void exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Turn();
			void exseq_main_region_Init_Init_process_Distant_signal_Turn_distant_Pending();
			void exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn();
			void exseq_main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending();
			void exseq_main_region_Wait_for_Start();
			void exseq_main_region_Operating();
			void exseq_main_region_Failed();
			void exseq_main_region();
			void exseq_main_region_Init_Init_process();
			void exseq_main_region_Init_Init_process_Main_signal_Turn_main();
			void exseq_main_region_Init_Init_process_Distant_signal_Turn_distant();
			void react_main_region_Init_Init_process__choice_0();
			void react_main_region_Init_Init_process__choice_1();
			void react_main_region_Init_Init_process__choice_2();
			void react_main_region__entry_Default();
			void react_main_region_Init_Init_process__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Main_signal_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Main_signal_Turn_main_Turn_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Main_signal_Turn_main_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Distant_signal_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Distant_signal_Turn_distant_Turn_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Distant_signal_Turn_distant_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Shunt_signal_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Shunt_signal_Turn_shunt_Turn_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Shunt_signal_Turn_shunt_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_for_Start_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised;

			/*! Indicates event 'queued' of default interface scope is active. */
			bool queued_raised;

			/*! Indicates event 'response' of default interface scope is active. */
			bool response_raised;

			/*! Indicates event 'fail' of default interface scope is active. */
			bool fail_raised;

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised;



		};


		inline SignalStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SIGNALSTATECHART_H_ */
