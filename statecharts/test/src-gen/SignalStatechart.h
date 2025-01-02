/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
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
#include "../common/sc_types.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_eventdriven.h"
#include "../common/sc_timer.h"
#include <string.h>

/*! \file
Header of the state machine 'SignalStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SignalStatechart : public sc::timer::TimedInterface, public sc::EventDrivenInterface
		{
		public:
			SignalStatechart() noexcept;

			virtual ~SignalStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Idle,
				main_region_Turning,
				main_region_Turning_Turn_processing_Pending,
				main_region_Turning_Turn_processing_Send,
				main_region_Fail
			};

			/*! The number of states. */
			static constexpr const sc::integer numStates {5};
			static constexpr const sc::integer scvi_main_region_Idle {0};
			static constexpr const sc::integer scvi_main_region_Turning {0};
			static constexpr const sc::integer scvi_main_region_Turning_Turn_processing_Pending {0};
			static constexpr const sc::integer scvi_main_region_Turning_Turn_processing_Send {0};
			static constexpr const sc::integer scvi_main_region_Fail {0};

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
				explicit  EventInstance(Event id) noexcept : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			template <typename T>
			class EventInstanceWithValue : public EventInstance
			{
			public:
				explicit  EventInstanceWithValue(Event id, T val) noexcept :
					EventInstance(id),
					value(val)
				{}
				virtual ~EventInstanceWithValue() = default;
				const T value;
			};
			/*! Raises the in event 'turn' of default interface scope. */
			void raiseTurn(sc::integer turn_);
			/*! Raises the in event 'queued' of default interface scope. */
			void raiseQueued();
			/*! Raises the in event 'ok' of default interface scope. */
			void raiseOk();
			/*! Raises the in event 'fail' of default interface scope. */
			void raiseFail();
			/*! Raises the in event 'clear' of default interface scope. */
			void raiseClear();
			/*! Check if event 'completed' of default interface scope is raised. */
			bool isRaisedCompleted() noexcept;
			/*! Check if event 'failed' of default interface scope is raised. */
			bool isRaisedFailed() noexcept;


			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			static sc::integer getTimeout()  noexcept;
			/*! Gets the value of the variable 'symbol' that is defined in the default interface scope. */
			sc::integer getSymbol() const noexcept;
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

				virtual bool hasSignal() = 0;

				virtual bool prepare() = 0;

				virtual void send() = 0;

				virtual void dump() = 0;


			};

			/*! Set the working instance of the operation callback interface 'OperationCallback'. */
			void setOperationCallback(OperationCallback * operationCallback) noexcept;

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
			void setTimerService(sc::timer::TimerServiceInterface * timerService_) noexcept override;

			sc::timer::TimerServiceInterface * getTimerService() noexcept override;

			void raiseTimeEvent(sc::eventid event) override;

			sc::integer getNumberOfParallelTimeEvents() noexcept override;



			/*! Checks if the specified state is active (until 2.4.1 the used method for states was calles isActive()). */
			bool isStateActive(State state) const noexcept;

			//! number of time events used by the state machine.
			static const sc::integer timeEventsCount {1};

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount {1};


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent() noexcept;

			bool dispatchEvent(EventInstance * event) noexcept;



		private:
			SignalStatechart(const SignalStatechart & rhs);
			SignalStatechart & operator=(const SignalStatechart &);

			static constexpr const sc::integer timeout {3500};
			sc::integer symbol {SignalStatechart::STOP};
			static constexpr const sc::integer OFF {-(1)};
			static constexpr const sc::integer STOP {0};
			static constexpr const sc::integer GO {1};


			bool modified {false};



			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates {1};

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];



			OperationCallback * ifaceOperationCallback;

			bool isExecuting {false};



			// prototypes of all internal functions

			void enact_main_region_Turning();
			void enact_main_region_Turning_Turn_processing_Send();
			void enact_main_region_Fail();
			void exact_main_region_Turning();
			void enseq_main_region_Idle_default();
			void enseq_main_region_Turning_default();
			void enseq_main_region_Turning_Turn_processing_Pending_default();
			void enseq_main_region_Turning_Turn_processing_Send_default();
			void enseq_main_region_Fail_default();
			void enseq_main_region_default();
			void enseq_main_region_Turning_Turn_processing_default();
			void exseq_main_region_Idle();
			void exseq_main_region_Turning();
			void exseq_main_region_Turning_Turn_processing_Pending();
			void exseq_main_region_Turning_Turn_processing_Send();
			void exseq_main_region_Fail();
			void exseq_main_region();
			void exseq_main_region_Turning_Turn_processing();
			void react_main_region_Turning_Turn_processing__choice_0();
			void react_main_region__choice_0();
			void react_main_region__entry_Default();
			void react_main_region_Turning_Turn_processing__entry_Default();
			sc::integer main_region_Idle_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_Turn_processing_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Turning_Turn_processing_Send_react(const sc::integer transitioned_before);
			sc::integer main_region_Fail_react(const sc::integer transitioned_before);
			void clearOutEvents() noexcept;
			void clearInEvents() noexcept;
			void microStep();
			void runCycle();


			/*! Sets the value of the variable 'symbol' that is defined in the default interface scope. */
			void setSymbol(sc::integer symbol) noexcept;
			/*! Sets the value of the variable 'modified' that is defined in the internal scope. */
			void setModified(bool modified) noexcept;

			/*! Indicates event 'turn' of default interface scope is active. */
			bool turn_raised {false};

			/*! Value of event 'turn' of default interface scope. */
			sc::integer turn_value {0};

			/*! Indicates event 'queued' of default interface scope is active. */
			bool queued_raised {false};

			/*! Indicates event 'ok' of default interface scope is active. */
			bool ok_raised {false};

			/*! Indicates event 'fail' of default interface scope is active. */
			bool fail_raised {false};

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised {false};

			/*! Indicates event 'completed' of default interface scope is active. */
			bool completed_raised {false};

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised {false};



		};


		inline SignalStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SIGNALSTATECHART_H_ */
