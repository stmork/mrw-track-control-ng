/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//
* */

#ifndef CROSSINGSTATECHART_H_
#define CROSSINGSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the CrossingStatechart state machine.
		*/
		class CrossingStatechart;
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
Header of the state machine 'CrossingStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class CrossingStatechart : public QObject, public sc::timer::TimedInterface, public std::enable_shared_from_this<sc::timer::TimedInterface>, public sc::EventDrivenInterface
		{
			Q_OBJECT

		public:
			explicit CrossingStatechart(QObject * parent) noexcept;

			virtual ~CrossingStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Wait_For_Start,
				main_region_Failed,
				main_region_Init,
				main_region_Operating,
				main_region_Operating_Processing_Unlocked,
				main_region_Operating_Processing_Locked,
				main_region_Operating_Processing_Pending,
				main_region_Operating_Processing_Pending_Crossing_processing_Closing,
				main_region_Operating_Processing_Pending_Crossing_processing_Opening,
				main_region_Operating_Processing_Pending_Crossing_processing_Delay
			};

			/*! The number of states. */
			static constexpr const sc::integer numStates {10};
			static constexpr const sc::integer scvi_main_region_Wait_For_Start {0};
			static constexpr const sc::integer scvi_main_region_Failed {0};
			static constexpr const sc::integer scvi_main_region_Init {0};
			static constexpr const sc::integer scvi_main_region_Operating {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Unlocked {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Locked {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Crossing_processing_Closing {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Crossing_processing_Opening {0};
			static constexpr const sc::integer scvi_main_region_Operating_Processing_Pending_Crossing_processing_Delay {0};

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				action,
				clear,
				start,
				response,
				failed,
				_te0_main_region_Init_,
				_te1_main_region_Operating_Processing_Pending_,
				_te2_main_region_Operating_Processing_Pending_Crossing_processing_Delay_
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
			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void inc() = 0;

				virtual void dec() = 0;

				virtual void unregister() = 0;

				virtual bool used() = 0;

				virtual void close() = 0;

				virtual void open() = 0;

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
			static const sc::integer timeEventsCount {3};

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount {2};


		public slots:
			/*! Slot for the in event 'action' that is defined in the default interface scope. */
			void action();

			/*! Slot for the in event 'clear' that is defined in the default interface scope. */
			void clear();

			/*! Slot for the in event 'start' that is defined in the default interface scope. */
			void start();

			/*! Slot for the in event 'response' that is defined in the default interface scope. */
			void response();

			/*! Slot for the in event 'failed' that is defined in the default interface scope. */
			void failed();


		protected:


			std::deque<std::unique_ptr<EventInstance>> incomingEventQueue;

			std::unique_ptr<EventInstance> getNextEvent() noexcept;

			bool dispatchEvent(std::unique_ptr<EventInstance> event) noexcept;


		private:
			CrossingStatechart(const CrossingStatechart & rhs);
			CrossingStatechart & operator=(const CrossingStatechart &);

			static constexpr const sc::integer timeout {3000};
			static constexpr const sc::integer delay {1500};



			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates {1};

			std::shared_ptr<sc::timer::TimerServiceInterface> timerService = {};
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];



			std::shared_ptr<OperationCallback> ifaceOperationCallback {nullptr};

			bool isExecuting {false};
			bool stateConfVectorChanged {false};



			// prototypes of all internal functions

			void enact_main_region_Failed();
			void enact_main_region_Init();
			void enact_main_region_Operating_Processing_Unlocked();
			void enact_main_region_Operating_Processing_Locked();
			void enact_main_region_Operating_Processing_Pending();
			void enact_main_region_Operating_Processing_Pending_Crossing_processing_Closing();
			void enact_main_region_Operating_Processing_Pending_Crossing_processing_Opening();
			void enact_main_region_Operating_Processing_Pending_Crossing_processing_Delay();
			void exact_main_region_Init();
			void exact_main_region_Operating_Processing_Pending();
			void exact_main_region_Operating_Processing_Pending_Crossing_processing_Delay();
			void enseq_main_region_Wait_For_Start_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_Init_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_Operating_Processing_Unlocked_default();
			void enseq_main_region_Operating_Processing_Locked_default();
			void enseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing_default();
			void enseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening_default();
			void enseq_main_region_Operating_Processing_Pending_Crossing_processing_Delay_default();
			void enseq_main_region_default();
			void enseq_main_region_Operating_Processing_default();
			void exseq_main_region_Wait_For_Start();
			void exseq_main_region_Failed();
			void exseq_main_region_Init();
			void exseq_main_region_Operating();
			void exseq_main_region_Operating_Processing_Unlocked();
			void exseq_main_region_Operating_Processing_Locked();
			void exseq_main_region_Operating_Processing_Pending();
			void exseq_main_region_Operating_Processing_Pending_Crossing_processing_Closing();
			void exseq_main_region_Operating_Processing_Pending_Crossing_processing_Opening();
			void exseq_main_region_Operating_Processing_Pending_Crossing_processing_Delay();
			void exseq_main_region();
			void exseq_main_region_Operating_Processing();
			void exseq_main_region_Operating_Processing_Pending_Crossing_processing();
			void react_main_region__entry_Default();
			void react_main_region_Operating_Processing__entry_Default();
			sc::integer main_region_Wait_For_Start_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Unlocked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Crossing_processing_Closing_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Crossing_processing_Opening_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Pending_Crossing_processing_Delay_react(const sc::integer transitioned_before);
			void clearInEvents() noexcept;
			void microStep();
			void runCycle();



			/*! Indicates event 'action' of default interface scope is active. */
			bool action_raised {false};

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised {false};

			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised {false};

			/*! Indicates event 'response' of default interface scope is active. */
			bool response_raised {false};

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised {false};


		};


		inline CrossingStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* CROSSINGSTATECHART_H_ */
