/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2025 Steffen A. Mork
//
* */

#ifndef OPERATINGMODESTATECHART_H_
#define OPERATINGMODESTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the OperatingModeStatechart state machine.
		*/
		class OperatingModeStatechart;
	}
}


#include <deque>
#include "../common/sc_types.h"
#include "../common/sc_statemachine.h"
#include "../common/sc_eventdriven.h"
#include "../common/sc_timer.h"
#include <string.h>

/*! \file
Header of the state machine 'OperatingModeStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class OperatingModeStatechart : public sc::timer::TimedInterface, public sc::EventDrivenInterface
		{
		public:
			OperatingModeStatechart() noexcept;

			virtual ~OperatingModeStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Exit,
				main_region__final_,
				main_region_Running,
				main_region_Running_operating_Failed,
				main_region_Running_operating_Prepare_Bus,
				main_region_Running_operating_Init,
				main_region_Running_operating_Operating,
				main_region_Running_operating_Editing,
				main_region_Running_operating_Disable,
				main_region_Manual,
				main_region_Wait
			};

			/*! The number of states. */
			static constexpr const sc::integer numStates {11};
			static constexpr const sc::integer scvi_main_region_Exit {0};
			static constexpr const sc::integer scvi_main_region__final_ {0};
			static constexpr const sc::integer scvi_main_region_Running {0};
			static constexpr const sc::integer scvi_main_region_Running_operating_Failed {0};
			static constexpr const sc::integer scvi_main_region_Running_operating_Prepare_Bus {0};
			static constexpr const sc::integer scvi_main_region_Running_operating_Init {0};
			static constexpr const sc::integer scvi_main_region_Running_operating_Operating {0};
			static constexpr const sc::integer scvi_main_region_Running_operating_Editing {0};
			static constexpr const sc::integer scvi_main_region_Running_operating_Disable {0};
			static constexpr const sc::integer scvi_main_region_Manual {0};
			static constexpr const sc::integer scvi_main_region_Wait {0};

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				clear,
				started,
				failed,
				edit,
				operate,
				manual,
				init,
				finalize,
				completed,
				Can_connected,
				_te0_main_region_Running_operating_Prepare_Bus_,
				_te1_main_region_Running_operating_Init_
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
			/*! Raises the in event 'clear' of default interface scope. */
			void raiseClear();
			/*! Raises the in event 'started' of default interface scope. */
			void raiseStarted();
			/*! Raises the in event 'failed' of default interface scope. */
			void raiseFailed();
			/*! Raises the in event 'edit' of default interface scope. */
			void raiseEdit();
			/*! Raises the in event 'operate' of default interface scope. */
			void raiseOperate();
			/*! Raises the in event 'manual' of default interface scope. */
			void raiseManual(bool manual_);
			/*! Raises the in event 'init' of default interface scope. */
			void raiseInit();
			/*! Raises the in event 'finalize' of default interface scope. */
			void raiseFinalize();
			/*! Raises the in event 'completed' of default interface scope. */
			void raiseCompleted();
			/*! Check if event 'start' of default interface scope is raised. */
			bool isRaisedStart() noexcept;
			/*! Check if event 'cleared' of default interface scope is raised. */
			bool isRaisedCleared() noexcept;
			/*! Check if event 'failing' of default interface scope is raised. */
			bool isRaisedFailing() noexcept;
			/*! Check if event 'operating' of default interface scope is raised. */
			bool isRaisedOperating() noexcept;
			/*! Get value of event 'operating' of default interface scope. */
			bool getOperatingValue() noexcept;
			/*! Check if event 'editing' of default interface scope is raised. */
			bool isRaisedEditing() noexcept;
			/*! Get value of event 'editing' of default interface scope. */
			bool getEditingValue() noexcept;
			/*! Check if event 'quitting' of default interface scope is raised. */
			bool isRaisedQuitting() noexcept;
			/*! Get value of event 'quitting' of default interface scope. */
			bool getQuittingValue() noexcept;
			/*! Check if event 'playing' of default interface scope is raised. */
			bool isRaisedPlaying() noexcept;
			/*! Get value of event 'playing' of default interface scope. */
			bool getPlayingValue() noexcept;
			/*! Check if event 'quit' of default interface scope is raised. */
			bool isRaisedQuit() noexcept;


			/*! Gets the value of the variable 'timeout' that is defined in the default interface scope. */
			static sc::integer getTimeout()  noexcept;
			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void resetTransaction() = 0;

				virtual bool isManualValid() = 0;

				virtual bool hasActiveRoutes() = 0;

				virtual void disableRoutes() = 0;

				virtual void activateManual(bool active) = 0;


			};

			/*! Set the working instance of the operation callback interface 'OperationCallback'. */
			void setOperationCallback(OperationCallback * operationCallback) noexcept;
			//! Inner class for can interface scope.
			class Can
			{
			public:
				explicit Can(OperatingModeStatechart * parent) noexcept;





				/*! Gets the value of the variable 'timeout' that is defined in the interface scope 'can'. */
				static sc::integer getTimeout()  noexcept;

				/*! Raises the in event 'connected' of interface scope 'can'. */
				void raiseConnected();

				//! Inner class for can interface scope operation callbacks.
				class OperationCallback
				{
				public:
					virtual ~OperationCallback() = 0;

					virtual void connectBus() = 0;

					virtual bool isConnected() = 0;


				};

				/*! Set the working instance of the operation callback interface 'OperationCallback'. */
				void setOperationCallback(OperationCallback * operationCallback) noexcept;


			private:
				friend class OperatingModeStatechart;

				static constexpr const sc::integer timeout {2500};

				/*! Indicates event 'connected' of interface scope 'can' is active. */
				bool connected_raised {false};

				OperatingModeStatechart * parent;



				OperationCallback * ifaceCanOperationCallback;


			};

			/*! Returns an instance of the interface class 'Can'. */
			Can & can() noexcept;


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
			static const sc::integer timeEventsCount {2};

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount {1};


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent() noexcept;

			bool dispatchEvent(EventInstance * event) noexcept;



		private:
			OperatingModeStatechart(const OperatingModeStatechart & rhs);
			OperatingModeStatechart & operator=(const OperatingModeStatechart &);

			static constexpr const sc::integer timeout {5000};



			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates {1};

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			Can ifaceCan {Can{nullptr}};

			OperationCallback * ifaceOperationCallback;

			bool isExecuting {false};



			// prototypes of all internal functions

			void enact_main_region_Exit();
			void enact_main_region_Running_operating_Failed();
			void enact_main_region_Running_operating_Prepare_Bus();
			void enact_main_region_Running_operating_Init();
			void enact_main_region_Running_operating_Operating();
			void enact_main_region_Running_operating_Editing();
			void enact_main_region_Running_operating_Disable();
			void enact_main_region_Manual();
			void enact_main_region_Wait();
			void exact_main_region_Exit();
			void exact_main_region_Running_operating_Prepare_Bus();
			void exact_main_region_Running_operating_Init();
			void exact_main_region_Running_operating_Operating();
			void exact_main_region_Running_operating_Editing();
			void exact_main_region_Manual();
			void enseq_main_region_Exit_default();
			void enseq_main_region__final__default();
			void enseq_main_region_Running_operating_Failed_default();
			void enseq_main_region_Running_operating_Prepare_Bus_default();
			void enseq_main_region_Running_operating_Init_default();
			void enseq_main_region_Running_operating_Operating_default();
			void enseq_main_region_Running_operating_Editing_default();
			void enseq_main_region_Running_operating_Disable_default();
			void enseq_main_region_Manual_default();
			void enseq_main_region_Wait_default();
			void enseq_main_region_default();
			void exseq_main_region_Exit();
			void exseq_main_region__final_();
			void exseq_main_region_Running();
			void exseq_main_region_Running_operating_Failed();
			void exseq_main_region_Running_operating_Prepare_Bus();
			void exseq_main_region_Running_operating_Init();
			void exseq_main_region_Running_operating_Operating();
			void exseq_main_region_Running_operating_Editing();
			void exseq_main_region_Running_operating_Disable();
			void exseq_main_region_Manual();
			void exseq_main_region();
			void exseq_main_region_Running_operating();
			void react_main_region__choice_0();
			void react_main_region_Running_operating__choice_0();
			void react_main_region__entry_Default();
			sc::integer main_region_Exit_react(const sc::integer transitioned_before);
			sc::integer main_region_Running_react(const sc::integer transitioned_before);
			sc::integer main_region_Running_operating_Failed_react(const sc::integer transitioned_before);
			sc::integer main_region_Running_operating_Prepare_Bus_react(const sc::integer transitioned_before);
			sc::integer main_region_Running_operating_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Running_operating_Operating_react(const sc::integer transitioned_before);
			sc::integer main_region_Running_operating_Editing_react(const sc::integer transitioned_before);
			sc::integer main_region_Running_operating_Disable_react(const sc::integer transitioned_before);
			sc::integer main_region_Manual_react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_react(const sc::integer transitioned_before);
			void clearOutEvents() noexcept;
			void clearInEvents() noexcept;
			void microStep();
			void runCycle();



			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised {false};

			/*! Indicates event 'started' of default interface scope is active. */
			bool started_raised {false};

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised {false};

			/*! Indicates event 'edit' of default interface scope is active. */
			bool edit_raised {false};

			/*! Indicates event 'operate' of default interface scope is active. */
			bool operate_raised {false};

			/*! Indicates event 'manual' of default interface scope is active. */
			bool manual_raised {false};

			/*! Value of event 'manual' of default interface scope. */
			bool manual_value {false};

			/*! Indicates event 'init' of default interface scope is active. */
			bool init_raised {false};

			/*! Indicates event 'finalize' of default interface scope is active. */
			bool finalize_raised {false};

			/*! Indicates event 'completed' of default interface scope is active. */
			bool completed_raised {false};

			/*! Indicates event 'start' of default interface scope is active. */
			bool start_raised {false};

			/*! Indicates event 'cleared' of default interface scope is active. */
			bool cleared_raised {false};

			/*! Indicates event 'failing' of default interface scope is active. */
			bool failing_raised {false};

			/*! Indicates event 'operating' of default interface scope is active. */
			bool operating_raised {false};

			/*! Value of event 'operating' of default interface scope. */
			bool operating_value {false};

			/*! Indicates event 'editing' of default interface scope is active. */
			bool editing_raised {false};

			/*! Value of event 'editing' of default interface scope. */
			bool editing_value {false};

			/*! Indicates event 'quitting' of default interface scope is active. */
			bool quitting_raised {false};

			/*! Value of event 'quitting' of default interface scope. */
			bool quitting_value {false};

			/*! Indicates event 'playing' of default interface scope is active. */
			bool playing_raised {false};

			/*! Value of event 'playing' of default interface scope. */
			bool playing_value {false};

			/*! Indicates event 'quit' of default interface scope is active. */
			bool quit_raised {false};



		};


		inline OperatingModeStatechart::OperationCallback::~OperationCallback() {}
		inline OperatingModeStatechart::Can::OperationCallback::~OperationCallback() {}

	}
}

#endif /* OPERATINGMODESTATECHART_H_ */
