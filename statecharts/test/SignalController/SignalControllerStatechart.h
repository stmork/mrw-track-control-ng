/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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
#include "../common/sc_types.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_timer.h"

/*! \file
Header of the state machine 'SignalControllerStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SignalControllerStatechart : public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
		public:
			SignalControllerStatechart();

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
				main_region_Operating_Processing_Shunting_State,
				main_region_Operating_Processing_Shunting_State_Processing_Idle,
				main_region_Operating_Processing_Shunting_State_Processing_Waiting,
				main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend,
				main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop,
				main_region_Operating_Processing_Tour_State,
				main_region_Operating_Processing_Tour_State_Processing_Waiting,
				main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main,
				main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant,
				main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant,
				main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay,
				main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt,
				main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend,
				main_region_Operating_Processing_Tour_State_Processing_Idle,
				main_region_Operating_Processing_Pending,
				main_region_Operating_Processing_Pending_Pending_Go_Main,
				main_region_Operating_Processing_Pending_Pending_Go_Distant,
				main_region_Operating_Processing_Pending_Pending_Go_Shunt,
				main_region_Operating_Processing_Pending_Pending_Delay,
				main_region_Failed
			};

			/*! The number of states. */
			static const sc::integer numStates = 31;
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
			static const sc::integer scvi_main_region_Operating_Processing_Shunting_State = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Shunting_State_Processing_Idle = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Idle = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Pending = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Pending_Pending_Go_Main = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Pending_Pending_Go_Distant = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Pending_Pending_Go_Shunt = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Pending_Pending_Delay = 0;
			static const sc::integer scvi_main_region_Failed = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				start,
				clear,
				failed,
				enable,
				extend,
				disable,
				completedMain,
				completedDistant,
				completedShunt,
				_te0_main_region_Init_,
				_te1_main_region_Operating_Processing_Shunting_State_Processing_Waiting_,
				_te2_main_region_Operating_Processing_Tour_State_Processing_Waiting_,
				_te3_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_,
				_te4_main_region_Operating_Processing_Pending_,
				_te5_main_region_Operating_Processing_Pending_Pending_Delay_
			};

			class EventInstance
			{
			public:
				explicit EventInstance(Event id) : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			/*! Raises the in event 'start' of default interface scope. */
			void raiseStart();
			/*! Raises the in event 'clear' of default interface scope. */
			void raiseClear();
			/*! Raises the in event 'failed' of default interface scope. */
			void raiseFailed();
			/*! Raises the in event 'enable' of default interface scope. */
			void raiseEnable();
			/*! Raises the in event 'extend' of default interface scope. */
			void raiseExtend();
			/*! Raises the in event 'disable' of default interface scope. */
			void raiseDisable();
			/*! Check if event 'turnMain' of default interface scope is raised. */
			bool isRaisedTurnMain();
			/*! Get value of event 'turnMain' of default interface scope. */
			sc::integer getTurnMainValue();
			/*! Check if event 'turnDistant' of default interface scope is raised. */
			bool isRaisedTurnDistant();
			/*! Get value of event 'turnDistant' of default interface scope. */
			sc::integer getTurnDistantValue();
			/*! Check if event 'turnShunt' of default interface scope is raised. */
			bool isRaisedTurnShunt();
			/*! Get value of event 'turnShunt' of default interface scope. */
			sc::integer getTurnShuntValue();
			/*! Raises the in event 'completedMain' of default interface scope. */
			void raiseCompletedMain();
			/*! Raises the in event 'completedDistant' of default interface scope. */
			void raiseCompletedDistant();
			/*! Raises the in event 'completedShunt' of default interface scope. */
			void raiseCompletedShunt();
			/*! Check if event 'started' of default interface scope is raised. */
			bool isRaisedStarted();
			/*! Check if event 'cleared' of default interface scope is raised. */
			bool isRaisedCleared();

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent();

			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			static sc::integer getTimeout() ;

			/*! Gets the value of the variable 'delay' that is defined in the default interface scope. */
			static sc::integer getDelay() ;

			/*! Gets the value of the variable 'symbol' that is defined in the default interface scope. */
			sc::integer getSymbol() const;

			/*! Sets the value of the variable 'symbol' that is defined in the default interface scope. */
			void setSymbol(sc::integer symbol);

			/*! Gets the value of the variable 'OFF' that is defined in the default interface scope. */
			static sc::integer getOFF() ;

			/*! Gets the value of the variable 'STOP' that is defined in the default interface scope. */
			static sc::integer getSTOP() ;

			/*! Gets the value of the variable 'GO' that is defined in the default interface scope. */
			static sc::integer getGO() ;

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void inc() = 0;

				virtual void dec() = 0;

				virtual bool hasMainSignal() = 0;

				virtual bool isMainAndShunt() = 0;

				virtual bool isLightSignal() = 0;

				virtual bool isTour() = 0;

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
			static const sc::integer timeEventsCount = 6;

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount = 2;


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			SignalControllerStatechart(const SignalControllerStatechart & rhs);
			SignalControllerStatechart & operator=(const SignalControllerStatechart &);

			static const sc::integer timeout;
			static const sc::integer delay;
			sc::integer symbol;
			static const sc::integer OFF;
			static const sc::integer STOP;
			static const sc::integer GO;


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
			void enact_main_region_Init_Init_process_Turning();
			void enact_main_region_Init_Init_process_Turning_main_Turn();
			void enact_main_region_Init_Init_process_Turning_distant_Turn();
			void enact_main_region_Init_Init_process_Turning_shunt_Turn();
			void enact_main_region_Operating();
			void enact_main_region_Operating_Processing_Unlocked();
			void enact_main_region_Operating_Processing_Shunting_State();
			void enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
			void enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend();
			void enact_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop();
			void enact_main_region_Operating_Processing_Tour_State();
			void enact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
			void enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main();
			void enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant();
			void enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant();
			void enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
			void enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt();
			void enact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend();
			void enact_main_region_Operating_Processing_Pending();
			void enact_main_region_Operating_Processing_Pending_Pending_Go_Main();
			void enact_main_region_Operating_Processing_Pending_Pending_Go_Distant();
			void enact_main_region_Operating_Processing_Pending_Pending_Go_Shunt();
			void enact_main_region_Operating_Processing_Pending_Pending_Delay();
			void enact_main_region_Failed();
			void exact_main_region_Init();
			void exact_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
			void exact_main_region_Operating_Processing_Tour_State_Processing_Waiting();
			void exact_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
			void exact_main_region_Operating_Processing_Pending();
			void exact_main_region_Operating_Processing_Pending_Pending_Delay();
			void exact_main_region_Failed();
			void enseq_main_region_Wait_for_Start_default();
			void enseq_main_region_Init_default();
			void enseq_main_region_Init_Init_process_Turning_default();
			void enseq_main_region_Init_Init_process_Turning_main_Turn_default();
			void enseq_main_region_Init_Init_process_Turning_main_Completed_default();
			void enseq_main_region_Init_Init_process_Turning_distant_Turn_default();
			void enseq_main_region_Init_Init_process_Turning_distant_Completed_default();
			void enseq_main_region_Init_Init_process_Turning_shunt_Turn_default();
			void enseq_main_region_Init_Init_process_Turning_shunt_Completed_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_Operating_Processing_Unlocked_default();
			void enseq_main_region_Operating_Processing_Shunting_State_Processing_Idle_default();
			void enseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend_default();
			void enseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Idle_default();
			void enseq_main_region_Operating_Processing_Pending_Pending_Go_Main_default();
			void enseq_main_region_Operating_Processing_Pending_Pending_Go_Distant_default();
			void enseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt_default();
			void enseq_main_region_Operating_Processing_Pending_Pending_Delay_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_default();
			void enseq_main_region_Init_Init_process_default();
			void enseq_main_region_Init_Init_process_Turning_main_default();
			void enseq_main_region_Init_Init_process_Turning_distant_default();
			void enseq_main_region_Init_Init_process_Turning_shunt_default();
			void enseq_main_region_Operating_Processing_default();
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
			void exseq_main_region_Operating_Processing_Shunting_State();
			void exseq_main_region_Operating_Processing_Shunting_State_Processing_Idle();
			void exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting();
			void exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend();
			void exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop();
			void exseq_main_region_Operating_Processing_Tour_State();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Idle();
			void exseq_main_region_Operating_Processing_Pending();
			void exseq_main_region_Operating_Processing_Pending_Pending_Go_Main();
			void exseq_main_region_Operating_Processing_Pending_Pending_Go_Distant();
			void exseq_main_region_Operating_Processing_Pending_Pending_Go_Shunt();
			void exseq_main_region_Operating_Processing_Pending_Pending_Delay();
			void exseq_main_region_Failed();
			void exseq_main_region();
			void exseq_main_region_Init_Init_process();
			void exseq_main_region_Init_Init_process_Turning_main();
			void exseq_main_region_Init_Init_process_Turning_distant();
			void exseq_main_region_Init_Init_process_Turning_shunt();
			void exseq_main_region_Operating_Processing();
			void exseq_main_region_Operating_Processing_Shunting_State_Processing();
			void exseq_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting();
			void exseq_main_region_Operating_Processing_Tour_State_Processing();
			void exseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting();
			void exseq_main_region_Operating_Processing_Pending_Pending();
			void react_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting__choice_0();
			void react_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting__choice_1();
			void react_main_region_Operating_Processing_Pending_Pending__choice_0();
			void react_main_region_Operating_Processing_Pending_Pending__choice_1();
			void react_main_region_Operating_Processing_Pending_Pending__choice_2();
			void react_main_region__entry_Default();
			void react_main_region_Init_Init_process_Turning_main__entry_Default();
			void react_main_region_Init_Init_process_Turning_distant__entry_Default();
			void react_main_region_Init_Init_process_Turning_shunt__entry_Default();
			void react_main_region_Init_Init_process__entry_Default();
			void react_main_region_Operating_Processing__entry_Default();
			void react_main_region_Init_Init_process__sync0();
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
			sc::integer main_region_Operating_Processing_Shunting_State_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Shunting_State_Processing_Idle_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Shunting_State_Processing_Waiting_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_Processing_Waiting_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Tour_State_Processing_Idle_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Pending_Go_Main_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Pending_Go_Distant_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Pending_Go_Shunt_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Pending_Delay_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			void clearOutEvents();
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised;

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised;

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised;

			/*! Indicates event 'enable' of default interface scope is active. */
			bool enable_raised;

			/*! Indicates event 'extend' of default interface scope is active. */
			bool extend_raised;

			/*! Indicates event 'disable' of default interface scope is active. */
			bool disable_raised;

			/*! Value of event 'turnMain' of default interface scope. */
			sc::integer turnMain_value;

			/*! Indicates event 'turnMain' of default interface scope is active. */
			bool turnMain_raised;

			/*! Value of event 'turnDistant' of default interface scope. */
			sc::integer turnDistant_value;

			/*! Indicates event 'turnDistant' of default interface scope is active. */
			bool turnDistant_raised;

			/*! Value of event 'turnShunt' of default interface scope. */
			sc::integer turnShunt_value;

			/*! Indicates event 'turnShunt' of default interface scope is active. */
			bool turnShunt_raised;

			/*! Indicates event 'completedMain' of default interface scope is active. */
			bool completedMain_raised;

			/*! Indicates event 'completedDistant' of default interface scope is active. */
			bool completedDistant_raised;

			/*! Indicates event 'completedShunt' of default interface scope is active. */
			bool completedShunt_raised;

			/*! Indicates event 'started' of default interface scope is active. */
			bool started_raised;

			/*! Indicates event 'cleared' of default interface scope is active. */
			bool cleared_raised;



		};


		inline SignalControllerStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SIGNALCONTROLLERSTATECHART_H_ */
