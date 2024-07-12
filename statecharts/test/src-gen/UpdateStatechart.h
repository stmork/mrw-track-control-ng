/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
//
* */

#ifndef UPDATESTATECHART_H_
#define UPDATESTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the UpdateStatechart state machine.
		*/
		class UpdateStatechart;
	}
}


#include <deque>
#include "../common/sc_types.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_eventdriven.h"
#include "../common/sc_timer.h"
#include <string.h>

/*! \file
Header of the state machine 'UpdateStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class UpdateStatechart : public sc::timer::TimedInterface, public sc::EventDrivenInterface
		{
		public:
			UpdateStatechart() noexcept;

			virtual ~UpdateStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Ping,
				main_region_Reset,
				main_region_Flash_Request,
				main_region_Flash_Complete_Page,
				main_region_Flash_Rest,
				main_region_Flash_Check,
				main_region_Leave_Bootloader,
				main_region_Booted,
				main_region_Wait_for_Connect,
				main_region_Test_Hardware_Mismatch,
				main_region_Failed
			};

			/*! The number of states. */
			static constexpr const sc::integer numStates {11};
			static constexpr const sc::integer scvi_main_region_Ping {0};
			static constexpr const sc::integer scvi_main_region_Reset {0};
			static constexpr const sc::integer scvi_main_region_Flash_Request {0};
			static constexpr const sc::integer scvi_main_region_Flash_Complete_Page {0};
			static constexpr const sc::integer scvi_main_region_Flash_Rest {0};
			static constexpr const sc::integer scvi_main_region_Flash_Check {0};
			static constexpr const sc::integer scvi_main_region_Leave_Bootloader {0};
			static constexpr const sc::integer scvi_main_region_Booted {0};
			static constexpr const sc::integer scvi_main_region_Wait_for_Connect {0};
			static constexpr const sc::integer scvi_main_region_Test_Hardware_Mismatch {0};
			static constexpr const sc::integer scvi_main_region_Failed {0};

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				connected,
				complete,
				mismatch,
				failed,
				_te0_main_region_Ping_,
				_te1_main_region_Reset_,
				_te2_main_region_Flash_Request_,
				_te3_main_region_Flash_Complete_Page_,
				_te4_main_region_Flash_Rest_,
				_te5_main_region_Flash_Check_,
				_te6_main_region_Leave_Bootloader_,
				_te7_main_region_Wait_for_Connect_,
				_te8_main_region_Test_Hardware_Mismatch_
			};

			class EventInstance
			{
			public:
				explicit  EventInstance(Event id) noexcept : eventId(id) {}
				virtual ~EventInstance() = default;
				const Event eventId;
			};
			/*! Raises the in event 'connected' of default interface scope. */
			void raiseConnected();
			/*! Raises the in event 'complete' of default interface scope. */
			void raiseComplete();
			/*! Raises the in event 'mismatch' of default interface scope. */
			void raiseMismatch();
			/*! Raises the in event 'failed' of default interface scope. */
			void raiseFailed();


			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			static sc::integer getTimeout()  noexcept;
			/*! Gets the value of the variable 'delay_boot' that is defined in the default interface scope. */
			static sc::integer getDelay_boot()  noexcept;
			/*! Gets the value of the variable 'delay_reset' that is defined in the default interface scope. */
			static sc::integer getDelay_reset()  noexcept;
			/*! Gets the value of the variable 'delay_flash_request' that is defined in the default interface scope. */
			static sc::integer getDelay_flash_request()  noexcept;
			/*! Gets the value of the variable 'delay_flash_page' that is defined in the default interface scope. */
			static sc::integer getDelay_flash_page()  noexcept;
			/*! Gets the value of the variable 'count' that is defined in the default interface scope. */
			sc::integer getCount() const noexcept;
			/*! Gets the value of the variable 'error' that is defined in the default interface scope. */
			sc::integer getError() const noexcept;
			/*! Gets the value of the variable 'retry' that is defined in the default interface scope. */
			static sc::integer getRetry()  noexcept;
			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void init(sc::integer count) = 0;

				virtual void ping() = 0;

				virtual void boot() = 0;

				virtual void flashRequest() = 0;

				virtual void flashCompletePage() = 0;

				virtual void flashRestPage() = 0;

				virtual void flashCheck() = 0;

				virtual void quit() = 0;

				virtual void fail(sc::integer code) = 0;

				virtual bool hasController() = 0;

				virtual bool hasPages() = 0;


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
			static const sc::integer timeEventsCount {9};

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount {1};


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent() noexcept;

			bool dispatchEvent(EventInstance * event) noexcept;



		private:
			UpdateStatechart(const UpdateStatechart & rhs);
			UpdateStatechart & operator=(const UpdateStatechart &);

			static constexpr const sc::integer timeout {250};
			static constexpr const sc::integer delay_boot {3000};
			static constexpr const sc::integer delay_reset {1200};
			static constexpr const sc::integer delay_flash_request {200};
			static constexpr const sc::integer delay_flash_page {60};
			sc::integer count {0};
			sc::integer error {0};
			static constexpr const sc::integer retry {10};



			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates {1};

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];



			OperationCallback * ifaceOperationCallback;

			bool isExecuting {false};



			// prototypes of all internal functions

			void enact_main_region_Ping();
			void enact_main_region_Reset();
			void enact_main_region_Flash_Request();
			void enact_main_region_Flash_Complete_Page();
			void enact_main_region_Flash_Rest();
			void enact_main_region_Flash_Check();
			void enact_main_region_Leave_Bootloader();
			void enact_main_region_Booted();
			void enact_main_region_Wait_for_Connect();
			void enact_main_region_Test_Hardware_Mismatch();
			void enact_main_region_Failed();
			void exact_main_region_Ping();
			void exact_main_region_Reset();
			void exact_main_region_Flash_Request();
			void exact_main_region_Flash_Complete_Page();
			void exact_main_region_Flash_Rest();
			void exact_main_region_Flash_Check();
			void exact_main_region_Leave_Bootloader();
			void exact_main_region_Wait_for_Connect();
			void exact_main_region_Test_Hardware_Mismatch();
			void enseq_main_region_Ping_default();
			void enseq_main_region_Reset_default();
			void enseq_main_region_Flash_Request_default();
			void enseq_main_region_Flash_Complete_Page_default();
			void enseq_main_region_Flash_Rest_default();
			void enseq_main_region_Flash_Check_default();
			void enseq_main_region_Leave_Bootloader_default();
			void enseq_main_region_Booted_default();
			void enseq_main_region_Wait_for_Connect_default();
			void enseq_main_region_Test_Hardware_Mismatch_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_default();
			void exseq_main_region_Ping();
			void exseq_main_region_Reset();
			void exseq_main_region_Flash_Request();
			void exseq_main_region_Flash_Complete_Page();
			void exseq_main_region_Flash_Rest();
			void exseq_main_region_Flash_Check();
			void exseq_main_region_Leave_Bootloader();
			void exseq_main_region_Booted();
			void exseq_main_region_Wait_for_Connect();
			void exseq_main_region_Test_Hardware_Mismatch();
			void exseq_main_region_Failed();
			void exseq_main_region();
			void react_main_region__choice_0();
			void react_main_region__choice_1();
			void react_main_region__choice_2();
			void react_main_region__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Ping_react(const sc::integer transitioned_before);
			sc::integer main_region_Reset_react(const sc::integer transitioned_before);
			sc::integer main_region_Flash_Request_react(const sc::integer transitioned_before);
			sc::integer main_region_Flash_Complete_Page_react(const sc::integer transitioned_before);
			sc::integer main_region_Flash_Rest_react(const sc::integer transitioned_before);
			sc::integer main_region_Flash_Check_react(const sc::integer transitioned_before);
			sc::integer main_region_Leave_Bootloader_react(const sc::integer transitioned_before);
			sc::integer main_region_Booted_react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_for_Connect_react(const sc::integer transitioned_before);
			sc::integer main_region_Test_Hardware_Mismatch_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			void clearInEvents() noexcept;
			void microStep();
			void runCycle();


			/*! Sets the value of the variable 'count' that is defined in the default interface scope. */
			void setCount(sc::integer count) noexcept;
			/*! Sets the value of the variable 'error' that is defined in the default interface scope. */
			void setError(sc::integer error) noexcept;

			/*! Indicates event 'connected' of default interface scope is active. */
			bool connected_raised {false};

			/*! Indicates event 'complete' of default interface scope is active. */
			bool complete_raised {false};

			/*! Indicates event 'mismatch' of default interface scope is active. */
			bool mismatch_raised {false};

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised {false};



		};


		inline UpdateStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* UPDATESTATECHART_H_ */
