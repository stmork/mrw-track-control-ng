/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//
* */

#ifndef SWITCHSTATECHART_H_
#define SWITCHSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the SwitchStatechart state machine.
		*/
		class SwitchStatechart;
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
Header of the state machine 'SwitchStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SwitchStatechart : public QObject, public sc::timer::TimedInterface, public std::enable_shared_from_this<sc::timer::TimedInterface>, public sc::EventDrivenInterface
		{
			Q_OBJECT

		public:
			explicit SwitchStatechart(QObject * parent) noexcept;

			virtual ~SwitchStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Wait_for_Start,
				main_region_Init,
				main_region_Operating,
				main_region_Operating_operating_Unlocked,
				main_region_Operating_operating_Locked,
				main_region_Operating_operating_Turning,
				main_region_Operating_operating_Turning_Turning_process_Turn_Right,
				main_region_Operating_operating_Turning_Turning_process_Turn_Left,
				main_region_Operating_operating_Turning_Turning_process_Pending,
				main_region_Failed
			};

			/*! The number of states. */
			static constexpr const sc::integer numStates {10};
			static constexpr const sc::integer scvi_main_region_Wait_for_Start {0};
			static constexpr const sc::integer scvi_main_region_Init {0};
			static constexpr const sc::integer scvi_main_region_Operating {0};
			static constexpr const sc::integer scvi_main_region_Operating_operating_Unlocked {0};
			static constexpr const sc::integer scvi_main_region_Operating_operating_Locked {0};
			static constexpr const sc::integer scvi_main_region_Operating_operating_Turning {0};
			static constexpr const sc::integer scvi_main_region_Operating_operating_Turning_Turning_process_Turn_Right {0};
			static constexpr const sc::integer scvi_main_region_Operating_operating_Turning_Turning_process_Turn_Left {0};
			static constexpr const sc::integer scvi_main_region_Operating_operating_Turning_Turning_process_Pending {0};
			static constexpr const sc::integer scvi_main_region_Failed {0};

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				clear,
				start,
				leftResponse,
				rightResponse,
				response,
				queued,
				failed,
				unlock,
				turn,
				_te0_main_region_Init_,
				_te1_main_region_Operating_operating_Turning_
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
			/*! Gets the value of the variable 'isManual' that is defined in the default interface scope. */
			bool getIsManual() const noexcept;
			/*! Sets the value of the variable 'isManual' that is defined in the default interface scope. */
			void setIsManual(bool isManual) noexcept;
			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void inc() = 0;

				virtual void dec() = 0;

				virtual void left() = 0;

				virtual void right() = 0;

				virtual void request() = 0;

				virtual bool doTurnLeft() = 0;

				virtual bool isFree() = 0;

				virtual bool isReserved() = 0;

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
			static const sc::integer timeEventsCount {2};

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount {1};


		public slots:
			/*! Slot for the in event 'clear' that is defined in the default interface scope. */
			void clear();

			/*! Slot for the in event 'start' that is defined in the default interface scope. */
			void start();

			/*! Slot for the in event 'leftResponse' that is defined in the default interface scope. */
			void leftResponse();

			/*! Slot for the in event 'rightResponse' that is defined in the default interface scope. */
			void rightResponse();

			/*! Slot for the in event 'response' that is defined in the default interface scope. */
			void response();

			/*! Slot for the in event 'queued' that is defined in the default interface scope. */
			void queued();

			/*! Slot for the in event 'failed' that is defined in the default interface scope. */
			void failed();

			/*! Slot for the in event 'unlock' that is defined in the default interface scope. */
			void unlock();

			/*! Slot for the in event 'turn' that is defined in the default interface scope. */
			void turn();


		signals:
			/*! Signal representing the out event 'started' that is defined in the default interface scope. */
			void started();

			/*! Signal representing the out event 'entered' that is defined in the default interface scope. */
			void entered();

			/*! Signal representing the out event 'stop' that is defined in the default interface scope. */
			void stop();


		protected:


			std::deque<std::unique_ptr<EventInstance>> incomingEventQueue;

			std::unique_ptr<EventInstance> getNextEvent() noexcept;

			bool dispatchEvent(std::unique_ptr<EventInstance> event) noexcept;



		private:
			SwitchStatechart(const SwitchStatechart & rhs);
			SwitchStatechart & operator=(const SwitchStatechart &);

			static constexpr const sc::integer timeout {2500};
			bool isManual {false};



			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates {1};

			std::shared_ptr<sc::timer::TimerServiceInterface> timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];



			std::shared_ptr<OperationCallback> ifaceOperationCallback {nullptr};

			bool isExecuting {false};



			// prototypes of all internal functions

			void enact_main_region_Init();
			void enact_main_region_Operating_operating_Unlocked();
			void enact_main_region_Operating_operating_Locked();
			void enact_main_region_Operating_operating_Turning();
			void enact_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
			void enact_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
			void enact_main_region_Failed();
			void exact_main_region_Init();
			void exact_main_region_Operating_operating_Turning();
			void enseq_main_region_Wait_for_Start_default();
			void enseq_main_region_Init_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_Operating_operating_Unlocked_default();
			void enseq_main_region_Operating_operating_Locked_default();
			void enseq_main_region_Operating_operating_Turning_mrw_statechart_SwitchStatechart_main_region_Operating_operating_Turning();
			void enseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right_default();
			void enseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left_default();
			void enseq_main_region_Operating_operating_Turning_Turning_process_Pending_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_default();
			void enseq_main_region_Operating_operating_default();
			void exseq_main_region_Wait_for_Start();
			void exseq_main_region_Init();
			void exseq_main_region_Operating();
			void exseq_main_region_Operating_operating_Unlocked();
			void exseq_main_region_Operating_operating_Locked();
			void exseq_main_region_Operating_operating_Turning();
			void exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Right();
			void exseq_main_region_Operating_operating_Turning_Turning_process_Turn_Left();
			void exseq_main_region_Operating_operating_Turning_Turning_process_Pending();
			void exseq_main_region_Failed();
			void exseq_main_region();
			void exseq_main_region_Operating_operating();
			void exseq_main_region_Operating_operating_Turning_Turning_process();
			void react_main_region_Operating_operating__choice_0();
			void react_main_region_Operating_operating_Turning_Turning_process__choice_0();
			void react_main_region__entry_Default();
			void react_main_region_Operating_operating__entry_Default();
			sc::integer main_region_Wait_for_Start_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Unlocked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Locked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Turning_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Turning_Turning_process_Turn_Right_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Turning_Turning_process_Turn_Left_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_operating_Turning_Turning_process_Pending_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			void clearInEvents() noexcept;
			void microStep();
			void runCycle();



			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised {false};

			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised {false};

			/*! Indicates event 'leftResponse' of default interface scope is active. */
			bool leftResponse_raised {false};

			/*! Indicates event 'rightResponse' of default interface scope is active. */
			bool rightResponse_raised {false};

			/*! Indicates event 'response' of default interface scope is active. */
			bool response_raised {false};

			/*! Indicates event 'queued' of default interface scope is active. */
			bool queued_raised {false};

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised {false};

			/*! Indicates event 'unlock' of default interface scope is active. */
			bool unlock_raised {false};

			/*! Indicates event 'turn' of default interface scope is active. */
			bool turn_raised {false};



		};


		inline SwitchStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SWITCHSTATECHART_H_ */
