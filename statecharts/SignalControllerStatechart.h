/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
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
#include "common/sc_eventdriven.h"
#include "common/sc_timer.h"
#include <memory>
#include <string.h>
#include <QObject>

/*! \file
Header of the state machine 'SignalControllerStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SignalControllerStatechart : public QObject, public sc::timer::TimedInterface, public std::enable_shared_from_this<sc::timer::TimedInterface>, public sc::EventDrivenInterface
		{
			Q_OBJECT

		public:
			explicit SignalControllerStatechart(QObject * parent) noexcept;

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
			static constexpr const sc::integer numStates {31};
			static constexpr const sc::integer scvi_main_region_Wait_for_Start {0};
			static constexpr const sc::integer scvi_main_region_Init {0};
			static constexpr const sc::integer scvi_main_region_Init_Init_process_Turning {0};
			static constexpr const sc::integer scvi_main_region_Init_Init_process_Turning_main_Turn {0};
			static constexpr const sc::integer scvi_main_region_Init_Init_process_Turning_main_Completed {0};
			static constexpr const sc::integer scvi_main_region_Init_Init_process_Turning_distant_Turn {1};
			static constexpr const sc::integer scvi_main_region_Init_Init_process_Turning_distant_Completed {1};
			static constexpr const sc::integer scvi_main_region_Init_Init_process_Turning_shunt_Turn {2};
			static constexpr const sc::integer scvi_main_region_Init_Init_process_Turning_shunt_Completed {2};
			static constexpr const sc::integer scvi_main_region_Operating {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Unlocked {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Shunting_State {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Shunting_State_Processing_Idle {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Extend {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Shunting_State_Processing_Waiting_Shunt_waiting_Stop {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Tour_State_Processing_Idle {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Pending_Go_Main {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Pending_Go_Distant {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Pending_Go_Shunt {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Pending_Delay {0};
			static constexpr const sc::integer scvi_main_region_Failed {0};

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
				explicit  EventInstance(Event id) noexcept : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};


			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			static sc::integer getTimeout()  noexcept;
			/*! Gets the value of the variable 'delay' that is defined in the default interface scope. */
			static sc::integer getDelay()  noexcept;
			/*! Gets the value of the variable 'symbol' that is defined in the default interface scope. */
			sc::integer getSymbol() const noexcept;
			/*! Sets the value of the variable 'symbol' that is defined in the default interface scope. */
			void setSymbol(sc::integer symbol) noexcept;
			/*! Gets the value of the variable 'OFF' that is defined in the default interface scope. */
			static sc::integer getOFF()  noexcept;
			/*! Gets the value of the variable 'STOP' that is defined in the default interface scope. */
			static sc::integer getSTOP()  noexcept;
			/*! Gets the value of the variable 'GO' that is defined in the default interface scope. */
			static sc::integer getGO()  noexcept;
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
			void setOperationCallback(std::shared_ptr<OperationCallback> operationCallback) noexcept;

			/*! Can be used by the client code to trigger a run to completion step without raising an event. */
			void triggerWithoutEvent() override;

			/*
			 * Functions inherited from StatemachineInterface
			 */
			void enter() override;

			void exit() override;

			/*!
			 * Checks if the state machine is active (until 2.4.1 this method was used for states).
			 * A state machine is active if it has been entered. It is inactive if it has not been entered at all or if it has been exited.
			 */
			bool isActive() const noexcept override;


			/*!
			* Checks if all active states are final.
			* If there are no active states then the state machine is considered being inactive. In this case this method returns false.
			*/
			bool isFinal() const noexcept override;

			/*!
			 * Checks if member of the state machine must be set. For example an operation callback.
			 */
			bool check() const noexcept;

			/*
			 * Functions inherited from TimedStatemachineInterface
			 */
			void setTimerService(std::shared_ptr<sc::timer::TimerServiceInterface> timerService_) noexcept override;

			std::shared_ptr<sc::timer::TimerServiceInterface> getTimerService() noexcept override;

			void raiseTimeEvent(sc::eventid event) override;

			sc::integer getNumberOfParallelTimeEvents() noexcept override;



			/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
			bool isStateActive(State state) const noexcept;

			//! number of time events used by the state machine.
			static const sc::integer timeEventsCount {6};

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount {2};


		public slots:
			/*! Slot for the in event 'start' that is defined in the default interface scope. */
			void start();

			/*! Slot for the in event 'clear' that is defined in the default interface scope. */
			void clear();

			/*! Slot for the in event 'failed' that is defined in the default interface scope. */
			void failed();

			/*! Slot for the in event 'enable' that is defined in the default interface scope. */
			void enable();

			/*! Slot for the in event 'extend' that is defined in the default interface scope. */
			void extend();

			/*! Slot for the in event 'disable' that is defined in the default interface scope. */
			void disable();

			/*! Slot for the in event 'completedMain' that is defined in the default interface scope. */
			void completedMain();

			/*! Slot for the in event 'completedDistant' that is defined in the default interface scope. */
			void completedDistant();

			/*! Slot for the in event 'completedShunt' that is defined in the default interface scope. */
			void completedShunt();


		signals:
			/*! Signal representing the out event 'turnMain' that is defined in the default interface scope. */
			void turnMain(sc::integer value);

			/*! Signal representing the out event 'turnDistant' that is defined in the default interface scope. */
			void turnDistant(sc::integer value);

			/*! Signal representing the out event 'turnShunt' that is defined in the default interface scope. */
			void turnShunt(sc::integer value);

			/*! Signal representing the out event 'started' that is defined in the default interface scope. */
			void started();

			/*! Signal representing the out event 'cleared' that is defined in the default interface scope. */
			void cleared();


		protected:


			std::deque<std::unique_ptr<EventInstance>> incomingEventQueue;

			std::unique_ptr<EventInstance> getNextEvent() noexcept;

			bool dispatchEvent(std::unique_ptr<EventInstance> event) noexcept;



		private:
			SignalControllerStatechart(const SignalControllerStatechart & rhs);
			SignalControllerStatechart & operator=(const SignalControllerStatechart &);

			static constexpr const sc::integer timeout {5000};
			static constexpr const sc::integer delay {350};
			sc::integer symbol {SignalControllerStatechart::STOP};
			static constexpr const sc::integer OFF {-(1)};
			static constexpr const sc::integer STOP {0};
			static constexpr const sc::integer GO {1};



			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates {3};

			std::shared_ptr<sc::timer::TimerServiceInterface> timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];



			std::shared_ptr<OperationCallback> ifaceOperationCallback {nullptr};

			bool isExecuting {false};
			sc::integer stateConfVectorPosition {0};
			bool stateConfVectorChanged {false};



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
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_mrw_statechart_SignalControllerStatechart_main_region_Operating_Processing_Tour_State_Processing_Waiting();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Main_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Distant_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Off_Distant_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Delay_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Stop_Shunt_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Waiting_Tour_waiting_Extend_default();
			void enseq_main_region_Operating_Processing_Tour_State_Processing_Idle_default();
			void enseq_main_region_Operating_Processing_Pending_mrw_statechart_SignalControllerStatechart_main_region_Operating_Processing_Pending();
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
			void clearInEvents() noexcept;
			void microStep();
			void runCycle();



			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised {false};

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised {false};

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised {false};

			/*! Indicates event 'enable' of default interface scope is active. */
			bool enable_raised {false};

			/*! Indicates event 'extend' of default interface scope is active. */
			bool extend_raised {false};

			/*! Indicates event 'disable' of default interface scope is active. */
			bool disable_raised {false};

			/*! Indicates event 'completedMain' of default interface scope is active. */
			bool completedMain_raised {false};

			/*! Indicates event 'completedDistant' of default interface scope is active. */
			bool completedDistant_raised {false};

			/*! Indicates event 'completedShunt' of default interface scope is active. */
			bool completedShunt_raised {false};



		};


		inline SignalControllerStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SIGNALCONTROLLERSTATECHART_H_ */
