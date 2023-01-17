/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#ifndef CONFIGSTATECHART_H_
#define CONFIGSTATECHART_H_

namespace mrw
{
	namespace statechart
	{
		/*!
		Forward declaration for the ConfigStatechart state machine.
		*/
		class ConfigStatechart;
	}
}


#include <deque>
#include "common/sc_types.h"
#include "common/sc_statemachine.h"
#include "common/sc_timer.h"
#include <QObject>

/*! \file
Header of the state machine 'ConfigStatechart'.
*/

namespace mrw
{
	namespace statechart
	{

		class ConfigStatechart : public QObject, public sc::timer::TimedInterface, public sc::StatemachineInterface
		{
			Q_OBJECT

		public:
			ConfigStatechart(QObject * parent);

			virtual ~ConfigStatechart();



			/*! Enumeration of all states. */
			enum class State
			{
				NO_STATE,
				main_region_Wait_for_Connect,
				main_region_Configure,
				main_region_Wait_for_Boot,
				main_region_Failed,
				main_region_Booted
			};

			/*! The number of states. */
			static const sc::integer numStates = 5;
			static const sc::integer scvi_main_region_Wait_for_Connect = 0;
			static const sc::integer scvi_main_region_Configure = 0;
			static const sc::integer scvi_main_region_Wait_for_Boot = 0;
			static const sc::integer scvi_main_region_Failed = 0;
			static const sc::integer scvi_main_region_Booted = 0;

			/*! Enumeration of all events which are consumed. */
			enum class Event
			{
				NO_EVENT,
				connected,
				sent,
				completed,
				failed,
				_te0_main_region_Wait_for_Connect_,
				_te1_main_region_Wait_for_Boot_
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
			static sc::integer getTimeout() ;

			/*! Gets the value of the variable 'flashtime' that is defined in the default interface scope. */
			static sc::integer getFlashtime() ;

			/*! Gets the value of the variable 'resettime' that is defined in the default interface scope. */
			static sc::integer getResettime() ;

			/*! Gets the value of the variable 'idx' that is defined in the default interface scope. */
			sc::integer getIdx() const;

			//! Inner class for default interface scope operation callbacks.
			class OperationCallback
			{
			public:
				virtual ~OperationCallback() = 0;

				virtual void configure(sc::integer idx) = 0;

				virtual bool hasMore(sc::integer idx) = 0;

				virtual void booting() = 0;

				virtual void quit() = 0;

				virtual void fail() = 0;


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
			/*! Slot for the in event 'connected' that is defined in the default interface scope. */
			void connected();

			/*! Slot for the in event 'sent' that is defined in the default interface scope. */
			void sent();

			/*! Slot for the in event 'completed' that is defined in the default interface scope. */
			void completed();

			/*! Slot for the in event 'failed' that is defined in the default interface scope. */
			void failed();


		protected:


			std::deque<EventInstance *> incomingEventQueue;

			EventInstance * getNextEvent();

			void dispatchEvent(EventInstance * event);



		private:
			ConfigStatechart(const ConfigStatechart & rhs);
			ConfigStatechart & operator=(const ConfigStatechart &);

			static const sc::integer timeout;
			static const sc::integer flashtime;
			static const sc::integer resettime;
			sc::integer idx;


			//! the maximum number of orthogonal states defines the dimension of the state configuration vector.
			static const sc::ushort maxOrthogonalStates = 1;

			sc::timer::TimerServiceInterface * timerService;
			bool timeEvents[timeEventsCount];


			State stateConfVector[maxOrthogonalStates];


			OperationCallback * ifaceOperationCallback;


			bool isExecuting;


			// prototypes of all internal functions

			void enact_main_region_Wait_for_Connect();
			void enact_main_region_Configure();
			void enact_main_region_Wait_for_Boot();
			void enact_main_region_Failed();
			void enact_main_region_Booted();
			void exact_main_region_Wait_for_Connect();
			void exact_main_region_Configure();
			void exact_main_region_Wait_for_Boot();
			void enseq_main_region_Wait_for_Connect_default();
			void enseq_main_region_Configure_default();
			void enseq_main_region_Wait_for_Boot_default();
			void enseq_main_region_Failed_default();
			void enseq_main_region_Booted_default();
			void enseq_main_region_default();
			void exseq_main_region_Wait_for_Connect();
			void exseq_main_region_Configure();
			void exseq_main_region_Wait_for_Boot();
			void exseq_main_region_Failed();
			void exseq_main_region_Booted();
			void exseq_main_region();
			void react_main_region__choice_0();
			void react_main_region__entry_Default();
			sc::integer react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_for_Connect_react(const sc::integer transitioned_before);
			sc::integer main_region_Configure_react(const sc::integer transitioned_before);
			sc::integer main_region_Wait_for_Boot_react(const sc::integer transitioned_before);
			sc::integer main_region_Failed_react(const sc::integer transitioned_before);
			sc::integer main_region_Booted_react(const sc::integer transitioned_before);
			void clearInEvents();
			void microStep();
			void runCycle();




			/*! Indicates event 'connected' of default interface scope is active. */
			bool connected_raised;

			/*! Indicates event 'sent' of default interface scope is active. */
			bool sent_raised;

			/*! Indicates event 'completed' of default interface scope is active. */
			bool completed_raised;

			/*! Indicates event 'failed' of default interface scope is active. */
			bool failed_raised;



		};


		inline ConfigStatechart::OperationCallback::~OperationCallback() {}

	}
}

#endif /* CONFIGSTATECHART_H_ */
