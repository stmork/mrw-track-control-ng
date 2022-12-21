/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//
* */

#ifndef SIGNALCONTROLLERSTATECHART_H_
#define SIGNALCONTROLLERSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the SignalControllerStatechart state machine.
		*/
		class SignalControllerStatechart;
	}
}


#include <deque>
#include "common/sc_types.h"
#include "common/sc_statemachine.h"
#include "common/sc_timer.h"
#include <QObject>

/*! \file
Header of the state machine 'SignalControllerStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SignalControllerStatechart : public QObject, public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
			Q_OBJECT

		public:
			SignalControllerStatechart(QObject * parent);

			virtual ~SignalControllerStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Wait_for_Start,
				main_region_Init,
				main_region_Init_Init_process_Turning,
				main_region_Init_Init_process_Turning_main_Turn,
				main_region_Init_Init_process_Turning_main_Completed,
				main_region_Init_Init_process_Turning_distant_Turn,
				main_region_Init_Init_process_Turning_distant_Completed,
				main_region_Init_Init_process_Turning_shunt_Turn,
				main_region_Init_Init_process_Turning_shunt_Completed,
				main_region_Operating,
				main_region_Operating_Processing_Unlocked,
				main_region_Operating_Processing_Locked,
				main_region_Operating_Processing_Locked_Processing_Idle,
				main_region_Operating_Processing_Locked_Processing_Waiting,
				main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Go,
				main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Stop,
				main_region_Failed
			};

			/*! The number of states. */
			static const sc::integer numStates = 17;
			static const sc::integer scvi_main_region_Wait_for_Start = 0;
			static const sc::integer scvi_main_region_Init = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Turning = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Turning_main_Turn = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Turning_main_Completed = 0;
			static const sc::integer scvi_main_region_Init_Init_process_Turning_distant_Turn = 1;
			static const sc::integer scvi_main_region_Init_Init_process_Turning_distant_Completed = 1;
			static const sc::integer scvi_main_region_Init_Init_process_Turning_shunt_Turn = 2;
			static const sc::integer scvi_main_region_Init_Init_process_Turning_shunt_Completed = 2;
			static const sc::integer scvi_main_region_Operating = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Unlocked = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked_Processing_Idle = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked_Processing_Waiting = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Go = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Stop = 0;
			static const sc::integer scvi_main_region_Failed = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				start,
				clear,
				started,
				failed,
				enable,
				disable,
				completedMain,
				completedDistant,
				completedShunt,
				_te0_main_region_Init_,
				_te1_main_region_Operating_Processing_Locked_Processing_Waiting_
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

				virtual void inc() = 0;

				virtual void dec() = 0;

				virtual bool hasMainSignal() = 0;

				virtual bool isMain() = 0;

				virtual void prepare() = 0;

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


		public slots:
			/*! Slot for the in event 'start' that is defined in the default interface scope. */
			void start();

			/*! Slot for the in event 'clear' that is defined in the default interface scope. */
			void clear();

			/*! Slot for the in event 'started' that is defined in the default interface scope. */
			void started();

			/*! Slot for the in event 'failed' that is defined in the default interface scope. */
			void failed();

			/*! Slot for the in event 'enable' that is defined in the default interface scope. */
			void enable();

			/*! Slot for the in event 'disable' that is defined in the default interface scope. */
			void disable();

			/*! Slot for the in event 'completedMain' that is defined in the default interface scope. */
			void completedMain();

			/*! Slot for the in event 'completedDistant' that is defined in the default interface scope. */
			void completedDistant();

			/*! Slot for the in event 'completedShunt' that is defined in the default interface scope. */
			void completedShunt();


		signals:
			/*! Signal representing the out event 'cleared' that is defined in the default interface scope. */
			void cleared();

			/*! Signal representing the out event 'stop' that is defined in the default interface scope. */
			void stop();

			/*! Signal representing the out event 'turnMain' that is defined in the default interface scope. */
			void turnMain(sc::integer value);

			/*! Signal representing the out event 'turnDistant' that is defined in the default interface scope. */
			void turnDistant(sc::integer value);

			/*! Signal representing the out event 'turnShunt' that is defined in the default interface scope. */
			void turnShunt(sc::integer value);


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			SignalControllerStatechart(const SignalControllerStatechart & rhs);
			SignalControllerStatechart & operator=(const SignalControllerStatechart &);

			sc::integer timeout;
			static const sc::integer OFF;
			static const sc::integer STOP;
			static const sc::integer GO;
			static const sc::integer SLOW;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 3;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;
			sc::integer stateConfVectorPosition;
			bool stateConfVectorChanged;


			// prototypes of all internal functions

			void enact_main_region_Init();
			void enact_main_region_Init_Init_process_Turning_main_Turn();
			void enact_main_region_Init_Init_process_Turning_distant_Turn();
			void enact_main_region_Init_Init_process_Turning_shunt_Turn();
			void enact_main_region_Operating();
			void enact_main_region_Operating_Processing_Unlocked();
			void enact_main_region_Operating_Processing_Locked();
			void enact_main_region_Operating_Processing_Locked_Processing_Waiting();
			void enact_main_region_Failed();
			void exact_main_region_Init();
			void exact_main_region_Operating_Processing_Locked_Processing_Waiting();
			void exact_main_region_Failed();
			void enseq_main_region_Wait_for_Start_default();
			void enseq_main_region_Init_default();
			void enseq_main_region_Init_Init_process_Turning_main_Turn_default();
			void enseq_main_region_Init_Init_process_Turning_main_Completed_default();
			void enseq_main_region_Init_Init_process_Turning_distant_Turn_default();
			void enseq_main_region_Init_Init_process_Turning_distant_Completed_default();
			void enseq_main_region_Init_Init_process_Turning_shunt_Turn_default();
			void enseq_main_region_Init_Init_process_Turning_shunt_Completed_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_Operating_Processing_Unlocked_default();
			void enseq_main_region_Operating_Processing_Locked_default();
			void enseq_main_region_Operating_Processing_Locked_Processing_Idle_default();
			void enseq_main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Go_default();
			void enseq_main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Stop_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_default();
			void enseq_main_region_Init_Init_process_default();
			void enseq_main_region_Operating_Processing_default();
			void enseq_main_region_Operating_Processing_Locked_Processing_default();
			void exseq_main_region_Wait_for_Start();
			void exseq_main_region_Init();
			void exseq_main_region_Init_Init_process_Turning();
			void exseq_main_region_Init_Init_process_Turning_main_Turn();
			void exseq_main_region_Init_Init_process_Turning_main_Completed();
			void exseq_main_region_Init_Init_process_Turning_distant_Turn();
			void exseq_main_region_Init_Init_process_Turning_distant_Completed();
			void exseq_main_region_Init_Init_process_Turning_shunt_Turn();
			void exseq_main_region_Init_Init_process_Turning_shunt_Completed();
			void exseq_main_region_Operating();
			void exseq_main_region_Operating_Processing_Unlocked();
			void exseq_main_region_Operating_Processing_Locked();
			void exseq_main_region_Operating_Processing_Locked_Processing_Idle();
			void exseq_main_region_Operating_Processing_Locked_Processing_Waiting();
			void exseq_main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Go();
			void exseq_main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Stop();
			void exseq_main_region_Failed();
			void exseq_main_region();
			void exseq_main_region_Init_Init_process();
			void exseq_main_region_Init_Init_process_Turning_main();
			void exseq_main_region_Init_Init_process_Turning_distant();
			void exseq_main_region_Init_Init_process_Turning_shunt();
			void exseq_main_region_Operating_Processing();
			void exseq_main_region_Operating_Processing_Locked_Processing();
			void exseq_main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing();
			void react_main_region__entry_Default();
			void react_main_region_Init_Init_process__entry_Default();
			void react_main_region_Operating_Processing__entry_Default();
			void react_main_region_Operating_Processing_Locked_Processing__entry_Default();
			void react_main_region_Init_Init_process__sync0();
			void react_main_region_Init_Init_process__sync1();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_for_Start_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Turning_main_Turn_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Turning_main_Completed_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Turning_distant_Turn_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Turning_distant_Completed_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Turning_shunt_Turn_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_process_Turning_shunt_Completed_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Unlocked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Processing_Idle_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Processing_Waiting_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Go_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Processing_Waiting_Turn_processing_Stop_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised;

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised;

			/*! Indicates event 'started' of default interface scope is active. */
			bool started_raised;

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised;

			/*! Indicates event 'enable' of default interface scope is active. */
			bool enable_raised;

			/*! Indicates event 'disable' of default interface scope is active. */
			bool disable_raised;

			/*! Indicates event 'completedMain' of default interface scope is active. */
			bool completedMain_raised;

			/*! Indicates event 'completedDistant' of default interface scope is active. */
			bool completedDistant_raised;

			/*! Indicates event 'completedShunt' of default interface scope is active. */
			bool completedShunt_raised;

			/*! Value of event 'turnMain' of default interface scope. */
			sc::integer turnMain_value;

			/*! Value of event 'turnDistant' of default interface scope. */
			sc::integer turnDistant_value;

			/*! Value of event 'turnShunt' of default interface scope. */
			sc::integer turnShunt_value;



		};


		inline SignalControllerStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SIGNALCONTROLLERSTATECHART_H_ */
