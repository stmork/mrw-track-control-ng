/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//
* */

#ifndef SECTIONSTATECHART_H_
#define SECTIONSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the SectionStatechart state machine.
		*/
		class SectionStatechart;
	}
}


#include <deque>
#include "common/sc_types.h"
#include "common/sc_statemachine.h"
#include "common/sc_timer.h"
#include <QObject>

/*! \file
Header of the state machine 'SectionStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class SectionStatechart : public QObject, public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
			Q_OBJECT

		public:
			SectionStatechart(QObject * parent);

			virtual ~SectionStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Init,
				main_region_Init_Init_Process_Requesting,
				main_region_Init_Init_Process_Requesting_relais_Relay,
				main_region_Init_Init_Process_Requesting_relais_Wait,
				main_region_Init_Init_Process_Requesting_state_Occupation,
				main_region_Init_Init_Process_Requesting_state_Wait,
				main_region_Operating,
				main_region_Operating_Processing_Unlocked,
				main_region_Operating_Processing_Locked,
				main_region_Operating_Processing_Locked_Lock_Handling_Enable,
				main_region_Operating_Processing_Locked_Lock_Handling_Enabled,
				main_region_Operating_Processing_Locked_Lock_Handling_Disable,
				main_region_Operating_Processing_Locked_Lock_Handling_Passed,
				main_region_Failed,
				main_region_Wait_for_Start
			};

			/*! The number of states. */
			static const sc::integer numStates = 15;
			static const sc::integer scvi_main_region_Init = 0;
			static const sc::integer scvi_main_region_Init_Init_Process_Requesting = 0;
			static const sc::integer scvi_main_region_Init_Init_Process_Requesting_relais_Relay = 0;
			static const sc::integer scvi_main_region_Init_Init_Process_Requesting_relais_Wait = 0;
			static const sc::integer scvi_main_region_Init_Init_Process_Requesting_state_Occupation = 1;
			static const sc::integer scvi_main_region_Init_Init_Process_Requesting_state_Wait = 1;
			static const sc::integer scvi_main_region_Operating = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Unlocked = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked_Lock_Handling_Enable = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked_Lock_Handling_Enabled = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked_Lock_Handling_Disable = 0;
			static const sc::integer scvi_main_region_Operating_Processing_Locked_Lock_Handling_Passed = 0;
			static const sc::integer scvi_main_region_Failed = 0;
			static const sc::integer scvi_main_region_Wait_for_Start = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				enable,
				disable,
				inquire,
				relaisResponse,
				stateResponse,
				clear,
				failed,
				_te0_main_region_Init_,
				_te1_main_region_Operating_Processing_Locked_Lock_Handling_Enable_,
				_te2_main_region_Operating_Processing_Locked_Lock_Handling_Disable_
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

			/*! Gets the value of the variable 'auto' that is defined in the default interface scope. */
			bool getAuto() const;

			/*! Sets the value of the variable 'auto' that is defined in the default interface scope. */
			void setAuto(bool auto_ID);

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void inc() = 0;

				virtual void dec() = 0;

				virtual void off() = 0;

				virtual void on() = 0;

				virtual void request() = 0;

				virtual void passed() = 0;

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
			static const sc::integer timeEventsCount = 3;

			//! number of time events that can be active at once.
			static const sc::integer parallelTimeEventsCount = 1;


		public slots:
			/*! Slot for the in event 'enable' that is defined in the default interface scope. */
			void enable();

			/*! Slot for the in event 'disable' that is defined in the default interface scope. */
			void disable();

			/*! Slot for the in event 'inquire' that is defined in the default interface scope. */
			void inquire();

			/*! Slot for the in event 'relaisResponse' that is defined in the default interface scope. */
			void relaisResponse();

			/*! Slot for the in event 'stateResponse' that is defined in the default interface scope. */
			void stateResponse(bool stateResponse_);

			/*! Slot for the in event 'clear' that is defined in the default interface scope. */
			void clear();

			/*! Slot for the in event 'failed' that is defined in the default interface scope. */
			void failed();


		signals:
			/*! Signal representing the out event 'inquired' that is defined in the default interface scope. */
			void inquired();

			/*! Signal representing the out event 'entered' that is defined in the default interface scope. */
			void entered();


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			SectionStatechart(const SectionStatechart & rhs);
			SectionStatechart & operator=(const SectionStatechart &);

			sc::integer timeout;
			bool auto__;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 2;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;
			sc::integer stateConfVectorPosition;
			bool stateConfVectorChanged;


			// prototypes of all internal functions

			void enact_main_region_Init();
			void enact_main_region_Init_Init_Process_Requesting_relais_Relay();
			void enact_main_region_Init_Init_Process_Requesting_state_Occupation();
			void enact_main_region_Operating_Processing_Unlocked();
			void enact_main_region_Operating_Processing_Locked();
			void enact_main_region_Operating_Processing_Locked_Lock_Handling_Enable();
			void enact_main_region_Operating_Processing_Locked_Lock_Handling_Disable();
			void enact_main_region_Operating_Processing_Locked_Lock_Handling_Passed();
			void exact_main_region_Init();
			void exact_main_region_Operating_Processing_Locked_Lock_Handling_Enable();
			void exact_main_region_Operating_Processing_Locked_Lock_Handling_Disable();
			void enseq_main_region_Init_default();
			void enseq_main_region_Init_Init_Process_Requesting_relais_Relay_default();
			void enseq_main_region_Init_Init_Process_Requesting_relais_Wait_default();
			void enseq_main_region_Init_Init_Process_Requesting_state_Occupation_default();
			void enseq_main_region_Init_Init_Process_Requesting_state_Wait_default();
			void enseq_main_region_Operating_default();
			void enseq_main_region_Operating_Processing_Unlocked_default();
			void enseq_main_region_Operating_Processing_Locked_default();
			void enseq_main_region_Operating_Processing_Locked_Lock_Handling_Enable_default();
			void enseq_main_region_Operating_Processing_Locked_Lock_Handling_Enabled_default();
			void enseq_main_region_Operating_Processing_Locked_Lock_Handling_Disable_default();
			void enseq_main_region_Operating_Processing_Locked_Lock_Handling_Passed_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_Wait_for_Start_default();
			void enseq_main_region_default();
			void enseq_main_region_Init_Init_Process_default();
			void enseq_main_region_Operating_Processing_default();
			void enseq_main_region_Operating_Processing_Locked_Lock_Handling_default();
			void exseq_main_region_Init();
			void exseq_main_region_Init_Init_Process_Requesting();
			void exseq_main_region_Init_Init_Process_Requesting_relais_Relay();
			void exseq_main_region_Init_Init_Process_Requesting_relais_Wait();
			void exseq_main_region_Init_Init_Process_Requesting_state_Occupation();
			void exseq_main_region_Init_Init_Process_Requesting_state_Wait();
			void exseq_main_region_Operating();
			void exseq_main_region_Operating_Processing_Unlocked();
			void exseq_main_region_Operating_Processing_Locked();
			void exseq_main_region_Operating_Processing_Locked_Lock_Handling_Enable();
			void exseq_main_region_Operating_Processing_Locked_Lock_Handling_Enabled();
			void exseq_main_region_Operating_Processing_Locked_Lock_Handling_Disable();
			void exseq_main_region_Operating_Processing_Locked_Lock_Handling_Passed();
			void exseq_main_region_Failed();
			void exseq_main_region_Wait_for_Start();
			void exseq_main_region();
			void exseq_main_region_Init_Init_Process();
			void exseq_main_region_Init_Init_Process_Requesting_relais();
			void exseq_main_region_Init_Init_Process_Requesting_state();
			void exseq_main_region_Operating_Processing();
			void exseq_main_region_Operating_Processing_Locked_Lock_Handling();
			void react_main_region_Operating_Processing_Locked_Lock_Handling__choice_0();
			void react_main_region__entry_Default();
			void react_main_region_Init_Init_Process__entry_Default();
			void react_main_region_Operating_Processing__entry_Default();
			void react_main_region_Operating_Processing_Locked_Lock_Handling__entry_Default();
			void react_main_region_Init_Init_Process__sync0();
			void react_main_region_Init_Init_Process__sync1();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Init_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_relais_Relay_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_relais_Wait_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_state_Occupation_react(const sc::integer transitioned_before);
			sc::integer main_region_Init_Init_Process_Requesting_state_Wait_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Unlocked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Lock_Handling_Enable_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Lock_Handling_Enabled_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Lock_Handling_Disable_react(const sc::integer transitioned_before);
			sc::integer main_region_Operating_Processing_Locked_Lock_Handling_Passed_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_for_Start_react(const sc::integer transitioned_before);
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'enable' of default interface scope is active. */
			bool enable_raised;

			/*! Indicates event 'disable' of default interface scope is active. */
			bool disable_raised;

			/*! Indicates event 'inquire' of default interface scope is active. */
			bool inquire_raised;

			/*! Indicates event 'relaisResponse' of default interface scope is active. */
			bool relaisResponse_raised;

			/*! Indicates event 'stateResponse' of default interface scope is active. */
			bool stateResponse_raised;

			/*! Value of event 'stateResponse' of default interface scope. */
			bool stateResponse_value;

			/*! Indicates event 'clear' of default interface scope is active. */
			bool clear_raised;

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised;



		};


		inline SectionStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* SECTIONSTATECHART_H_ */
